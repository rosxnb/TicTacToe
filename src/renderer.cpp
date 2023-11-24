#include "renderer.hpp"
#include "utility.hpp"
#include "keys_code.h"

Renderer::Renderer( MTL::Device* pDevice )
    : p_device( pDevice->retain() )
    , p_cmdQ( p_device->newCommandQueue() )
{ 
    build_shaders();
    build_buffers();
}

Renderer::~Renderer()
{
    p_boardVertices->release();
    p_boradColors->release();
    p_boardIndices->release();
    p_lineColors->release();
    p_lineVertices->release();

    p_shaderLibrary->release();
    p_RPS->release();
    p_cmdQ->release();
    p_device->release();
}

void Renderer::build_shaders()
{
    using NS::StringEncoding::UTF8StringEncoding;

    m_shaderSrc = Utility::read_file("shader/program.metal");

    NS::Error* pError {nullptr};
    p_shaderLibrary = p_device->newLibrary( NS::String::string(m_shaderSrc.data(), UTF8StringEncoding), nullptr, &pError );
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
    float len = 0.92f;
    const simd::float3 boardColors = { 0.50f, 0.50f, 0.50f };
    const simd::float3 boardVertices[] = 
    {
        {  len,  len,  0.0f }, // 1st quadrant
        { -len,  len,  0.0f }, // 2nd quadrant
        { -len, -len,  0.0f }, // 3rd quadrant
        {  len, -len,  0.0f }, // 4th quadrant
    };
    const uint16_t boardIndices[] = {
        0, 1, 2,
        2, 3, 0
    };

    p_boardVertices = p_device->newBuffer( sizeof(boardVertices), MTL::ResourceStorageModeManaged );
    p_boradColors = p_device->newBuffer( sizeof(boardColors), MTL::ResourceStorageModeManaged );
    p_boardIndices = p_device->newBuffer( sizeof(boardIndices), MTL::ResourceStorageModeManaged );

    memcpy( p_boardVertices->contents(), &boardVertices, sizeof(boardVertices) );
    memcpy( p_boradColors->contents(), &boardColors, sizeof(boardColors) );
    memcpy( p_boardIndices->contents(), &boardIndices, sizeof(boardIndices) );

    p_boardVertices->didModifyRange( NS::Range::Make(0, p_boardVertices->length()) );
    p_boradColors->didModifyRange( NS::Range::Make(0, p_boradColors->length()) );
    p_boardIndices->didModifyRange( NS::Range::Make(0, p_boardIndices->length()) );


    // ------------------------ Vertical Lines ------------------------------------------------------------------
    const simd::float3 lineColors = { 0.01f, 0.01f, 0.01f };
    float width = 0.04f;
    std::vector<simd::float3> lineVertices;

    Point2D p1 = { -0.33f,  len };
    Point2D p2 = { -0.33f, -len };
    std::vector<simd::float3> verticalLineA = generate_line_vertices( p1, p2, width );
    lineVertices.insert( lineVertices.end(), verticalLineA.begin(), verticalLineA.end() );

    p1 = { 0.33f,  len };
    p2 = { 0.33f, -len };
    std::vector<simd::float3> verticalLineB = generate_line_vertices( p1, p2, width );
    lineVertices.insert( lineVertices.end(), verticalLineB.begin(), verticalLineB.end() );


    // ------------------------ Horizontal Lines ------------------------------------------------------------------
    p1 = {  len, -0.33f };
    p2 = { -len, -0.33f };
    std::vector<simd::float3> horizontalLineA = generate_line_vertices( p1, p2, width );
    lineVertices.insert( lineVertices.end(), horizontalLineA.begin(), horizontalLineA.end() );

    p1 = {  len, 0.33f };
    p2 = { -len, 0.33f };
    std::vector<simd::float3> horizontalLineB = generate_line_vertices( p1, p2, width );
    lineVertices.insert( lineVertices.end(), horizontalLineB.begin(), horizontalLineB.end() );


    p_lineColors = p_device->newBuffer( sizeof(lineColors), MTL::ResourceStorageModeManaged );
    p_lineVertices = p_device->newBuffer( lineVertices.size() * sizeof(simd::float3), MTL::ResourceStorageModeManaged );

    memcpy( p_lineColors->contents(), &lineColors, sizeof(lineColors) );
    memcpy( p_lineVertices->contents(), lineVertices.data(), lineVertices.size() * sizeof(simd::float3) );

    p_lineColors->didModifyRange( NS::Range::Make(0, p_lineColors->length()) );
    p_lineVertices->didModifyRange( NS::Range::Make(0, p_lineVertices->length()) );
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
    NS::AutoreleasePool* pPool = NS::AutoreleasePool::alloc()->init();
    process_keybord_inputs();

    MTL::CommandBuffer* pCmdBuf = p_cmdQ->commandBuffer();
    MTL::RenderPassDescriptor* pRPD = pView->currentRenderPassDescriptor();
    MTL::RenderCommandEncoder* pEnc = pCmdBuf->renderCommandEncoder( pRPD );

    // ----------------------- Game Board -----------------------------------------------------
    pEnc->setRenderPipelineState( p_RPS );
    pEnc->setVertexBuffer( p_boardVertices, 0, 0 );
    pEnc->setVertexBuffer( p_boradColors, 0, 1 );
    pEnc->drawIndexedPrimitives(
        MTL::PrimitiveType::PrimitiveTypeTriangle,
        NS::UInteger(6), // index count
        MTL::IndexType::IndexTypeUInt16, // index type
        p_boardIndices, // indices data
        NS::UInteger(0) // index buffer offset
    );

    // -------------------------- Tiles Seperation Lines ---------------------------------------
    pEnc->setRenderPipelineState( p_RPS );
    pEnc->setVertexBuffer( p_lineVertices, 0, 0 );
    pEnc->setVertexBuffer( p_lineColors, 0, 1 );
    pEnc->drawPrimitives(
        MTL::PrimitiveType::PrimitiveTypeTriangle,
        NS::UInteger(0), // vertex start
        NS::UInteger(24)  // vertex count
    );


    pEnc->endEncoding();
    pCmdBuf->presentDrawable( pView->currentDrawable() );
    pCmdBuf->commit();
    pPool->release();
}
