#include "renderer.hpp"
#include <simd/simd.h>
#include <fstream>
#include <sstream>
#include <memory>

Renderer::Renderer( MTL::Device* pDevice )
    : _pDevice( pDevice->retain() )
    , _pCmdQ( _pDevice->newCommandQueue() )
{ 
    build_shaders();
    build_buffers();
}

Renderer::~Renderer()
{
    delete _pShaderSrc; 
    
    _pLineArgs->release();
    _pLineColors->release();
    _pLinePoints->release();
    _pIndices->release();
    _pColors->release();
    _pPositions->release();
    _pArgBuff->release();
    _pLib->release();
    _pRps->release();
    _pCmdQ->release();
    _pDevice->release();
}

std::string* Renderer::read_source(const char* filepath) const
{
    std::string* buffer = new std::string;

    std::ifstream fileHandler;
    fileHandler.exceptions( std::ifstream::badbit | std::ifstream::failbit );

    try 
    {
        fileHandler.open(filepath);
        std::stringstream sstream;
        sstream << fileHandler.rdbuf();
        buffer->assign( sstream.str() );
    }
    catch (std::ifstream::failure err)
    {
        __builtin_printf("Failed to load file: %s \n", filepath);
        __builtin_printf("%s \n\n", err.what());
    }

    return buffer;
}

void Renderer::build_shaders()
{
    using NS::StringEncoding::UTF8StringEncoding;

    NS::Error* pError {nullptr};
    const char* shaderSrc = read_source("shader/program.metal")->data();
    _pLib = _pDevice->newLibrary( NS::String::string(shaderSrc, UTF8StringEncoding), nullptr, &pError );
    if ( !_pLib )
    {
        __builtin_printf("Library creation from shader source failed. \n");
        __builtin_printf("%s \n\n", pError->localizedDescription()->utf8String());
        assert( false );
    }

    MTL::Function* fnVertex     = _pLib->newFunction( NS::String::string("main_vertex", UTF8StringEncoding) );
    MTL::Function* fnFragment   = _pLib->newFunction( NS::String::string("main_fragment", UTF8StringEncoding) );
    MTL::RenderPipelineDescriptor* pRpd = MTL::RenderPipelineDescriptor::alloc()->init();

    pRpd->setVertexFunction( fnVertex );
    pRpd->setFragmentFunction( fnFragment );
    pRpd->colorAttachments()->object(0)->setPixelFormat( MTL::PixelFormat::PixelFormatBGRA8Unorm_sRGB );

    _pRps = _pDevice->newRenderPipelineState( pRpd, &pError );
    if ( !_pRps )
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
    using simd::float3;

    constexpr size_t N = 4;
    float3 positions[N] = 
    {
        { -0.5f,  0.7f,  0.0f },    // top left
        { -0.5f, -0.7f,  0.0f },    // bottom left
        {  0.5f, -0.7f,  0.0f },    // bottom right
        {  0.5f,  0.7f,  0.0f }     // top right
    };
    float3 colors[N] = 
    {
        { 0.1f, 0.2f, 0.1f },
        { 0.1f, 0.2f, 0.1f },
        { 0.1f, 0.2f, 0.1f },
        { 0.1f, 0.2f, 0.1f },
    };
    uint16_t indices[] = 
    {
        0, 1, 2,
        2, 3, 0
    };

    float3 points[] = 
    {
        { -1.f,  0.f,  0.f },
        {  1.f,  0.f,  0.f },
        {  0.f, -1.f,  0.f },
        {  0.f,  1.f,  0.f }
    };
    float3 lineColor[] = 
    {
        { 0.f, 0.f, 0.f },
        { 0.f, 0.f, 0.f },
        { 0.f, 0.f, 0.f },
        { 0.f, 0.f, 0.f }
    };

    _pPositions   = _pDevice->newBuffer( sizeof(positions), MTL::ResourceStorageModeManaged );
    _pColors      = _pDevice->newBuffer( sizeof(colors), MTL::ResourceStorageModeManaged );
    _pIndices     = _pDevice->newBuffer( sizeof(indices), MTL::ResourceStorageModeManaged );
    _pLinePoints  = _pDevice->newBuffer( sizeof(points), MTL::ResourceStorageModeManaged );
    _pLineColors  = _pDevice->newBuffer( sizeof(lineColor), MTL::ResourceStorageModeManaged );

    memcpy( _pPositions->contents(), positions, sizeof(positions) );
    memcpy( _pColors->contents(), colors, sizeof(colors) );
    memcpy( _pIndices->contents(), indices, sizeof(indices) );
    memcpy( _pLinePoints->contents(), points, sizeof(points) );
    memcpy( _pLineColors->contents(), lineColor, sizeof(lineColor) );

    _pPositions->didModifyRange( NS::Range::Make(0 , _pPositions->length()) );
    _pColors->didModifyRange( NS::Range::Make(0, _pColors->length()) );
    _pIndices->didModifyRange( NS::Range::Make(0, _pIndices->length()) );
    _pLinePoints->didModifyRange( NS::Range::Make(0, _pLinePoints->length()) );
    _pLineColors->didModifyRange( NS::Range::Make(0, _pLineColors->length()) );

    assert( _pLib );
    MTL::Function* fnVertex = _pLib->newFunction( NS::String::string("main_vertex", NS::StringEncoding::UTF8StringEncoding) );

    MTL::ArgumentEncoder* pArgEnc = fnVertex->newArgumentEncoder( 0 );
    {
        _pArgBuff = _pDevice->newBuffer( pArgEnc->encodedLength(), MTL::ResourceStorageModeManaged );

        pArgEnc->setArgumentBuffer( _pArgBuff, 0 );
        pArgEnc->setBuffer( _pPositions, 0, 0 );
        pArgEnc->setBuffer( _pColors, 0, 1 );

        _pArgBuff->didModifyRange( NS::Range::Make(0, _pArgBuff->length()) );
    }

    MTL::ArgumentEncoder* pLineEnc = fnVertex->newArgumentEncoder( 0 );
    {
        _pLineArgs = _pDevice->newBuffer( pLineEnc->encodedLength(), MTL::ResourceStorageModeManaged );

        pLineEnc->setArgumentBuffer( _pLineArgs, 0 );
        pLineEnc->setBuffer( _pLinePoints, 0, 0 );
        pLineEnc->setBuffer( _pLineColors, 0, 1 );

        _pLineArgs->didModifyRange( NS::Range::Make(0, _pLineArgs->length()) );
    }

    fnVertex->release();
    pArgEnc->release();
    pLineEnc->release();
}

void Renderer::draw( MTK::View* pView )
{
    NS::AutoreleasePool* pPool = NS::AutoreleasePool::alloc()->init();

    MTL::CommandBuffer* pCmdBuff = _pCmdQ->commandBuffer();

    MTL::RenderPassDescriptor* pRpd = pView->currentRenderPassDescriptor();
    MTL::RenderCommandEncoder* pEncSq = pCmdBuff->renderCommandEncoder( pRpd );
    {
        pEncSq->setRenderPipelineState( _pRps );
        pEncSq->setVertexBuffer( _pArgBuff, 0, 0 );
        pEncSq->useResource( _pPositions, MTL::ResourceUsageRead );
        pEncSq->useResource( _pColors, MTL::ResourceUsageRead );

        // void drawIndexedPrimitives( PrimitiveType primitiveType, NS::UInteger indexCount, IndexType indexType,
        //                             const class Buffer* pIndexBuffer, NS::UInteger indexBufferOffset );
        pEncSq->drawIndexedPrimitives( MTL::PrimitiveType::PrimitiveTypeTriangle,
                6,
                MTL::IndexType::IndexTypeUInt16,
                _pIndices,
                0 );

        pEncSq->endEncoding();
    }

    MTL::RenderPassDescriptor* pRpdLine = MTL::RenderPassDescriptor::alloc()->init();
    pRpdLine->colorAttachments()->object(0)->setTexture( pRpd->colorAttachments()->object(0)->texture() );
    MTL::RenderCommandEncoder* pEncLine = pCmdBuff->renderCommandEncoder( pRpdLine );
    {
        pEncLine->setRenderPipelineState( _pRps );
        pEncLine->setVertexBuffer( _pLineArgs, 0, 0 );
        pEncLine->useResource( _pLinePoints, MTL::ResourceUsageRead );
        pEncLine->useResource( _pLineColors, MTL::ResourceUsageRead );
        pEncLine->drawPrimitives( MTL::PrimitiveType::PrimitiveTypeLine,
                                  NS::UInteger(0),
                                  NS::UInteger(4) );

        pEncLine->endEncoding();
    }

    pCmdBuff->presentDrawable( pView->currentDrawable() );
    pCmdBuff->commit();

    pRpdLine->release();
    pPool->release();
}
