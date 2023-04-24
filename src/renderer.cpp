#include <fstream>
#include <sstream>
#include <memory>

#include "renderer.hpp"
#include "keys_code.h"

Renderer::Renderer( MTL::Device* pDevice, std::unique_ptr<std::string> pShaderSrc )
    : _pDevice( pDevice->retain() )
    , _pCmdQ( _pDevice->newCommandQueue() )
    , _pShaderSrc( std::move(pShaderSrc) )
{ 
    // build_shaders();
    // build_buffers();
}

Renderer::~Renderer()
{
    _pLib->release();
    _pRps->release();
    _pCmdQ->release();
    _pDevice->release();
}

void Renderer::build_shaders()
{
    using NS::StringEncoding::UTF8StringEncoding;

    NS::Error* pError {nullptr};
    _pLib = _pDevice->newLibrary( NS::String::string(_pShaderSrc->data(), UTF8StringEncoding), nullptr, &pError );
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
}

void Renderer::draw( MTK::View* pView )
{
    NS::AutoreleasePool* pPool = NS::AutoreleasePool::alloc()->init();

    if (checkKeyPress( KeysCode::W ))
        __builtin_printf("Key W \n");
    if (checkKeyPress( KeysCode::S ))
        __builtin_printf("Key S \n");
    if (checkKeyPress( KeysCode::D ))
        __builtin_printf("Key D \n");
    if (checkKeyPress( KeysCode::A ))
        __builtin_printf("Key A \n");

    MTL::CommandBuffer* pCmdBuf = _pCmdQ->commandBuffer();

    MTL::RenderPassDescriptor* pRPD = pView->currentRenderPassDescriptor();
    MTL::RenderCommandEncoder* pEnc = pCmdBuf->renderCommandEncoder( pRPD );

    pEnc->endEncoding();
    pCmdBuf->presentDrawable( pView->currentDrawable() );
    pCmdBuf->commit();

    pPool->release();
}
