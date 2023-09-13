#include "renderer.hpp"
#include "keys_code.h"

Renderer::Renderer( MTL::Device* pDevice, std::unique_ptr<std::string> pShaderSrc )
    : p_device( pDevice->retain() )
    , p_cmdQ( p_device->newCommandQueue() )
    , p_shaderSrc( std::move(pShaderSrc) )
{ 
    build_shaders();
    build_buffers();
}

Renderer::~Renderer()
{
    p_vertices->release();
    p_colors->release();
    p_indices->release();
    p_line1->release();
    p_lineColor->release();

    p_shaderLibrary->release();
    p_RPS->release();
    p_cmdQ->release();
    p_device->release();
}

void Renderer::build_shaders()
{
    using NS::StringEncoding::UTF8StringEncoding;

    NS::Error* pError {nullptr};
    p_shaderLibrary = p_device->newLibrary( NS::String::string(p_shaderSrc->data(), UTF8StringEncoding), nullptr, &pError );
    if ( !p_shaderLibrary )
    {
        __builtin_printf("Library creation from shader source failed. \n");
        __builtin_printf("%s \n\n", pError->localizedDescription()->utf8String());
        assert( false );
    }

    MTL::Function* fnVertex     = p_shaderLibrary->newFunction( NS::String::string("main_vertex", UTF8StringEncoding) );
    MTL::Function* fnFragment   = p_shaderLibrary->newFunction( NS::String::string("main_fragment", UTF8StringEncoding) );
    MTL::RenderPipelineDescriptor* pRpd = MTL::RenderPipelineDescriptor::alloc()->init();

    pRpd->setVertexFunction( fnVertex );
    pRpd->setFragmentFunction( fnFragment );
    pRpd->colorAttachments()->object(0)->setPixelFormat( MTL::PixelFormat::PixelFormatBGRA8Unorm_sRGB );

    p_RPS = p_device->newRenderPipelineState( pRpd, &pError );
    if ( !p_RPS )
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
    // board vertices
    float len = 0.8f;
    const simd::float3 vertices_board[] = 
    {
        {  len,  len,  0.0f }, // 1st quadrant
        { -len,  len,  0.0f }, // 2nd quadrant
        { -len, -len,  0.0f }, // 3rd quadrant
        {  len, -len,  0.0f }, // 4th quadrant
    };

    // const simd::float3 colors = { 1.0f, 0.0f, 0.0f };
    const simd::float3 colors[] = {
        { 1.0f, 0.0f, 0.0f },
        { 1.0f, 0.0f, 0.0f },
        { 1.0f, 0.0f, 0.0f },
    };

    const uint16_t indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    p_vertices = p_device->newBuffer( sizeof(vertices_board), MTL::ResourceStorageModeManaged );
    p_colors = p_device->newBuffer( sizeof(colors), MTL::ResourceStorageModeManaged );
    p_indices = p_device->newBuffer( sizeof(indices), MTL::ResourceStorageModeManaged );

    memcpy( p_vertices->contents(), &vertices_board, sizeof(vertices_board) );
    memcpy( p_colors->contents(), &colors, sizeof(colors) );
    memcpy( p_indices->contents(), &indices, sizeof(indices) );

    p_vertices->didModifyRange( NS::Range::Make(0, p_vertices->length()) );
    p_colors->didModifyRange( NS::Range::Make(0, p_colors->length()) );
    p_indices->didModifyRange( NS::Range::Make(0, p_indices->length()) );

    // ----------------------------------------------------------------------------------------------------------

    const simd::float3 lineColors = { 0.0f, 1.0f, 0.0f };
    Point2D p1 = { -0.4f,  0.5f };
    Point2D p2 = { -0.4f, -0.5f };
    float width = 0.04f;
    std::vector<simd::float3> line = generate_line_vertices( p1, p2, width );

    p_line1 = p_device->newBuffer( line.size() * sizeof(simd::float3), MTL::ResourceStorageModeManaged );
    p_lineColor = p_device->newBuffer( sizeof(lineColors), MTL::ResourceStorageModeManaged );

    memcpy( p_line1->contents(), line.data(), line.size() * sizeof(simd::float3) );
    memcpy( p_lineColor->contents(), &lineColors, sizeof(lineColors) );

    p_line1->didModifyRange( NS::Range::Make(0, p_line1->length()) );
    p_lineColor->didModifyRange( NS::Range::Make(0, p_lineColor->length()) );
}

void Renderer::process_keybord_inputs()
{
    if (checkKeyPress( KeysCode::W ))
        __builtin_printf("Key W \n");
    if (checkKeyPress( KeysCode::S ))
        __builtin_printf("Key S \n");
    if (checkKeyPress( KeysCode::D ))
        __builtin_printf("Key D \n");
    if (checkKeyPress( KeysCode::A ))
        __builtin_printf("Key A \n");
}

std::vector<simd::float3> Renderer::generate_line_vertices(Point2D p1, Point2D p2, float width)
{
    std::vector<simd::float3> buffer;
    if (p1.x - p2.x == 0) // vertical line
    {
        buffer.push_back( simd::float3{ p1.x,         p1.y, 0.f } );
        buffer.push_back( simd::float3{ p2.x,         p2.y, 0.f } );
        buffer.push_back( simd::float3{ p1.x + width, p1.y, 0.f } );
        buffer.push_back( simd::float3{ p2.x,         p2.y, 0.f } );
        buffer.push_back( simd::float3{ p2.x + width, p2.y, 0.f } );
        buffer.push_back( simd::float3{ p1.x + width, p1.y, 0.f } );
    }
    else if (p1.y - p2.y == 0) // horizontal line
    {
        buffer.push_back( simd::float3{ p1.x,         p1.y, 0.f } );
        buffer.push_back( simd::float3{ p2.x,         p2.y, 0.f } );
        buffer.push_back( simd::float3{ p1.x, p1.y + width, 0.f } );
        buffer.push_back( simd::float3{ p2.x,         p2.y, 0.f } );
        buffer.push_back( simd::float3{ p2.x, p2.y + width, 0.f } );
        buffer.push_back( simd::float3{ p1.x, p1.y + width, 0.f } );
    }
    // TODO
    // else // tilted line
    // {
    //
    // }
    return buffer;
}

void Renderer::draw( MTK::View* pView )
{
    process_keybord_inputs();

    { // draw the square
        NS::AutoreleasePool* pPool = NS::AutoreleasePool::alloc()->init();
        MTL::CommandBuffer* pCmdBuf = p_cmdQ->commandBuffer();

        MTL::RenderPassDescriptor* pRPD = pView->currentRenderPassDescriptor();
        MTL::RenderCommandEncoder* pEnc = pCmdBuf->renderCommandEncoder( pRPD );

        pEnc->setRenderPipelineState( p_RPS );
        pEnc->setVertexBuffer( p_vertices, 0, 0 );
        pEnc->setVertexBuffer( p_colors, 0, 1 );

        //
        // void drawIndexedPrimitives( PrimitiveType primitiveType, NS::UInteger indexCount, IndexType indexType,
        //                             const class Buffer* pIndexBuffer, NS::UInteger indexBufferOffset );
        pEnc->drawIndexedPrimitives(
            MTL::PrimitiveType::PrimitiveTypeTriangle,
            NS::UInteger(6),
            MTL::IndexType::IndexTypeUInt16,
            p_indices,
            NS::UInteger(0)
        );

        // pEnc->endEncoding();
        // pCmdBuf->presentDrawable( pView->currentDrawable() );
        // pCmdBuf->commit();
        // pPool->release();
    // }
    //
    //
    // { // Draw the line on top
    //     NS::AutoreleasePool* pPool = NS::AutoreleasePool::alloc()->init();
    //     MTL::CommandBuffer* pCmdBuf = p_cmdQ->commandBuffer();
    //
    //     MTL::RenderPassDescriptor* pRPD = pView->currentRenderPassDescriptor();
    //     MTL::RenderCommandEncoder* pEnc = pCmdBuf->renderCommandEncoder( pRPD );

        pEnc->setRenderPipelineState( p_RPS );
        pEnc->setVertexBuffer( p_line1, 0, 0 );
        pEnc->setVertexBuffer( p_lineColor, 0, 1 );

        pEnc->drawPrimitives(
            MTL::PrimitiveType::PrimitiveTypeTriangle,
            NS::UInteger(0),
            NS::UInteger(6)
        );

        pEnc->endEncoding();
        pCmdBuf->presentDrawable( pView->currentDrawable() );
        pCmdBuf->commit();
        pPool->release();
    }
}
