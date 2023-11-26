#include "renderer.hpp"
#include "math.hpp"
#include "utility.hpp"
#include "keys_code.h"
#include <algorithm>

Renderer::Renderer( MTL::Device* pDevice )
    : p_device( pDevice->retain() )
    , p_cmdQueue( p_device->newCommandQueue() )
{ 
    build_shaders();
    build_buffers();

    m_semaphore = dispatch_semaphore_create( Renderer::kMaxFrames );
    m_moves.fill(0);
}

Renderer::~Renderer()
{
    p_vertices->release();
    p_indices->release();
    p_projection->release();
    for (size_t i = 0; i < Renderer::kMaxFrames; ++i)
    {
        p_instances[i]->release();
    }

    p_shaderLibrary->release();
    p_renderPipeline->release();
    p_cmdQueue->release();
    p_device->release();
}

void Renderer::build_shaders()
{
    using NS::StringEncoding::UTF8StringEncoding;

    m_shaderSrc = utility::read_file("shader/program.metal");

    NS::Error* pError {nullptr};
    p_shaderLibrary = p_device->newLibrary( NS::String::string(m_shaderSrc.data(), UTF8StringEncoding), nullptr, &pError );
    if ( !p_shaderLibrary )
    {
        __builtin_printf("Library creation from shader source failed. \n");
        __builtin_printf("%s \n\n", pError->localizedDescription()->utf8String());
        assert( false );
    }

    MTL::Function* fnVertex             = p_shaderLibrary->newFunction( NS::String::string("main_vertex", UTF8StringEncoding) );
    MTL::Function* fnFragment           = p_shaderLibrary->newFunction( NS::String::string("main_fragment", UTF8StringEncoding) );
    MTL::RenderPipelineDescriptor* pRpd = MTL::RenderPipelineDescriptor::alloc()->init();

    pRpd->setVertexFunction( fnVertex );
    pRpd->setFragmentFunction( fnFragment );
    pRpd->colorAttachments()->object(0)->setPixelFormat( MTL::PixelFormat::PixelFormatBGRA8Unorm_sRGB );
    pRpd->colorAttachments()->object(0)->setBlendingEnabled(true);
    pRpd->colorAttachments()->object(0)->setSourceRGBBlendFactor(MTL::BlendFactorSourceAlpha);
    pRpd->colorAttachments()->object(0)->setDestinationRGBBlendFactor(MTL::BlendFactorOneMinusSourceAlpha);
    pRpd->colorAttachments()->object(0)->setSourceAlphaBlendFactor(MTL::BlendFactorSourceAlpha);
    pRpd->colorAttachments()->object(0)->setDestinationAlphaBlendFactor(MTL::BlendFactorOneMinusSourceAlpha);

    p_renderPipeline = p_device->newRenderPipelineState( pRpd, &pError );
    if ( !p_renderPipeline )
    {
        __builtin_printf("RenderPipelineState creation failed. \n");
        __builtin_printf("%s \n\n", pError->localizedDescription()->utf8String());
        assert( false );
    }

    fnVertex->release();
    fnFragment->release();
    pRpd->release();
}

void Renderer::build_buffers()
{
    simd::float3 vertices[] = {
        {       0.f,             window_height , 0.f }, // top left
        { tileWidth,             window_height , 0.f }, // top right
        { tileWidth, window_height - tileHeight, 0.f }, // bottom right
        {       0.f, window_height - tileHeight, 0.f }, // bottom left
    };

    uint16_t indices[] = {
        0, 1, 2,
        2, 3, 0,
    };

    size_t verticesSize = sizeof(vertices);
    size_t indicesSize  = 6 * sizeof(uint16_t);
    p_vertices = p_device->newBuffer( verticesSize, MTL::ResourceStorageModeManaged );
    p_indices  = p_device->newBuffer( indicesSize, MTL::ResourceStorageModeManaged );

    memcpy( p_vertices->contents(), vertices, verticesSize );
    memcpy( p_indices->contents(), indices, indicesSize );

    p_vertices->didModifyRange( NS::Range::Make(0, p_vertices->length()) );
    p_indices->didModifyRange( NS::Range::Make(0, p_indices->length()) );

    // allocate space for instances data
    for (size_t i = 0; i < Renderer::kMaxFrames; ++i)
    {
        p_instances[i] = p_device->newBuffer( (Renderer::kNumInstances + 1) * sizeof(shader_types::InstanceData), MTL::ResourceStorageModeManaged );
    }

    // prepare projection matrix
    simd::float4x4 orthoProj = math::make_orthographic(0.f, window_width, 0.f, window_height, -1.f, 1.f);
    p_projection = p_device->newBuffer( sizeof(orthoProj), MTL::ResourceStorageModeManaged );
    memcpy( p_projection->contents(), &orthoProj, sizeof(orthoProj) );
    p_projection->didModifyRange( NS::Range::Make(0, p_projection->length()) );
}

void Renderer::draw( MTK::View* pView )
{
    if (checkKeyPress(KeysCode::ReturnOrEnter))
    {
        size_t selected_tile = get_selected_tile();

        if (m_moves[selected_tile] == 0)
        {
            m_moves[selected_tile] = m_player;
            m_player = m_player == 1 ? 2 : 1;
        }

        if ( tictactoe_has_winner() )
            m_winner = m_player == 1 ? 2 : 1;
    }

    NS::AutoreleasePool* pPool = NS::AutoreleasePool::alloc()->init();

    m_frame = (m_frame + 1) % Renderer::kMaxFrames;
    MTL::Buffer* pCurInstanceBuffer = p_instances[m_frame];

    dispatch_semaphore_wait( m_semaphore, DISPATCH_TIME_FOREVER );

    MTL::CommandBuffer* pCmdBuffer = p_cmdQueue->commandBuffer();

    pCmdBuffer->addCompletedHandler( [this] (MTL::CommandBuffer* pCmdBuf) {
        dispatch_semaphore_signal( this->m_semaphore );
    } );

    
    uint xTileNo = 1;
    uint yTileNo = 1;
    auto curInstanceData = reinterpret_cast< shader_types::InstanceData * >( pCurInstanceBuffer->contents() );

    for (size_t i = 0; i < Renderer::kNumInstances; ++i)
    {
        if ( xTileNo > 3 )
        {
            xTileNo = 1;
            yTileNo++;
        }

        simd::float3 translateVec { get_x_translate_value(xTileNo), -get_y_translate_value(yTileNo) };
        curInstanceData[ i ].translate = math::make_translate( translateVec );
        curInstanceData[ i ].player = m_moves[ i ];

        xTileNo++;
    }

    // handle selector tile
    {
        size_t idx = Renderer::kNumInstances; // last tile is selector tile at idx = 9

        std::tie(xTileNo, yTileNo) = process_keyboard_input();
        simd::float3 translateVec { get_x_translate_value(xTileNo), -get_y_translate_value(yTileNo) };
        curInstanceData[ idx ].translate = math::make_translate( translateVec );
        curInstanceData[ idx ].player = 0;
    }

    pCurInstanceBuffer->didModifyRange( NS::Range::Make(0, pCurInstanceBuffer->length()) );

    MTL::RenderPassDescriptor* pRPD = pView->currentRenderPassDescriptor();
    MTL::RenderCommandEncoder* pCmdEncoder = pCmdBuffer->renderCommandEncoder( pRPD );

    pCmdEncoder->setRenderPipelineState( p_renderPipeline );
    pCmdEncoder->setVertexBuffer( p_vertices, 0, 0 );
    pCmdEncoder->setVertexBuffer( pCurInstanceBuffer, 0, 1 );
    pCmdEncoder->setVertexBuffer( p_projection, 0, 2 );
    pCmdEncoder->setFragmentBytes( &m_winner, sizeof(m_winner), 0 );

    // void drawIndexedPrimitives( PrimitiveType primitiveType, NS::UInteger indexCount, IndexType indexType,
    //                             const class Buffer* pIndexBuffer, NS::UInteger indexBufferOffset, NS::UInteger instanceCount );
    pCmdEncoder->drawIndexedPrimitives( MTL::PrimitiveType::PrimitiveTypeTriangle,
                                        6, MTL::IndexType::IndexTypeUInt16,
                                        p_indices,
                                        0,
                                        kNumInstances + 1 );

    pCmdEncoder->endEncoding();
    pCmdBuffer->presentDrawable( pView->currentDrawable() );
    pCmdBuffer->commit();
    pPool->release();
}

std::pair<size_t, size_t> Renderer::process_keyboard_input()
{
    bool go_up     = checkKeyPress(KeysCode::W) || checkKeyPress(KeysCode::K) || checkKeyPress(KeysCode::UpArrow);
    bool go_left   = checkKeyPress(KeysCode::A) || checkKeyPress(KeysCode::H) || checkKeyPress(KeysCode::LeftArrow);
    bool go_down   = checkKeyPress(KeysCode::S) || checkKeyPress(KeysCode::J) || checkKeyPress(KeysCode::DownArrow);
    bool go_right  = checkKeyPress(KeysCode::D) || checkKeyPress(KeysCode::L) || checkKeyPress(KeysCode::RightArrow);

    if        (go_up)      h_curr_row -= 0.19; /*, __builtin_printf("up \n"); */
    else if   (go_down)    h_curr_row += 0.19; /*, __builtin_printf("down \n"); */
    else if   (go_right)   h_curr_col += 0.19; /*, __builtin_printf("right \n"); */
    else if   (go_left)    h_curr_col -= 0.19; /*, __builtin_printf("left \n"); */

    if (h_curr_col >= 4.)       h_curr_col = 1.;
    else if (h_curr_col < 1.f)  h_curr_col = 3.8;

    if (h_curr_row >= 4.)       h_curr_row = 1.;
    else if (h_curr_row < 1.)   h_curr_row = 3.8;

    return { static_cast<size_t>(h_curr_col), static_cast<size_t>(h_curr_row) };
}

size_t Renderer::get_selected_tile()
{
    size_t row = static_cast< size_t >( h_curr_row );
    size_t col = static_cast< size_t >( h_curr_col );
    size_t idx = 3 * (row - 1) + (col - 1);

    // __builtin_printf("selected tile: %ld \n", idx);

    return idx;
}

bool Renderer::tictactoe_has_winner() const
{
    if (    std::all_of(m_moves.begin(), m_moves.begin() + 3, [](int tile) { return tile == 1; })
         || std::all_of(m_moves.begin(), m_moves.begin() + 3, [](int tile) { return tile == 2; }) )
        return true;

    if (    std::all_of(m_moves.begin() + 3, m_moves.begin() + 6, [](int tile) { return tile == 1; })
         || std::all_of(m_moves.begin() + 3, m_moves.begin() + 6, [](int tile) { return tile == 2; }) )
        return true;

    if (    std::all_of(m_moves.begin() + 6, m_moves.end(), [](int tile) { return tile == 1; })
         || std::all_of(m_moves.begin() + 6, m_moves.end(), [](int tile) { return tile == 2; }) )
        return true;

    if (    (m_moves[0] == 1 && m_moves[3] == 1 && m_moves[6] == 1)
         || (m_moves[0] == 2 && m_moves[3] == 2 && m_moves[6] == 2) )
        return true;

    if (    (m_moves[1] == 1 && m_moves[4] == 1 && m_moves[7] == 1)
         || (m_moves[1] == 2 && m_moves[4] == 2 && m_moves[7] == 2) )
        return true;

    if (    (m_moves[2] == 1 && m_moves[5] == 1 && m_moves[8] == 1)
         || (m_moves[2] == 2 && m_moves[5] == 2 && m_moves[8] == 2) )
        return true;

    if (    (m_moves[0] == 1 && m_moves[4] == 1 && m_moves[8] == 1)
         || (m_moves[0] == 2 && m_moves[4] == 2 && m_moves[8] == 2) )
        return true;

    if (    (m_moves[2] == 1 && m_moves[4] == 1 && m_moves[6] == 1)
         || (m_moves[2] == 2 && m_moves[4] == 2 && m_moves[6] == 2) )
        return true;

    return false;
}

bool Renderer::tictactoe_terminal_state() const
{
    if ( tictactoe_has_winner() ) return true;

    for (int tile: m_moves)
        if (tile == 0) return false;

    return true;
}


// std::pair<size_t, size_t> Renderer::process_keyboard_input()
// {
//     bool go_up     = checkKeyPress(KeysCode::W) || checkKeyPress(KeysCode::K) || checkKeyPress(KeysCode::UpArrow);
//     bool go_left   = checkKeyPress(KeysCode::A) || checkKeyPress(KeysCode::H) || checkKeyPress(KeysCode::LeftArrow);
//     bool go_down   = checkKeyPress(KeysCode::S) || checkKeyPress(KeysCode::J) || checkKeyPress(KeysCode::DownArrow);
//     bool go_right  = checkKeyPress(KeysCode::D) || checkKeyPress(KeysCode::L) || checkKeyPress(KeysCode::RightArrow);
//
//     if        (go_up)      h_curr_row--, __builtin_printf("up \n");
//     else if   (go_down)    h_curr_row++, __builtin_printf("down \n");
//     else if   (go_right)   h_curr_col++, __builtin_printf("right \n");
//     else if   (go_left)    h_curr_col--, __builtin_printf("left \n");
//
//
//     if (h_curr_col > 3)
//     {
//         h_curr_col = 1;
//         h_curr_row++;
//     }
//     else if (h_curr_col < 1) h_curr_col = 1;
//
//     if (h_curr_row > 3 || h_curr_row < 1) {
//         h_curr_row = 1;
//     }
//
//     return { h_curr_col, h_curr_row };
// }
