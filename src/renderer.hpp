#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <Metal/Metal.hpp>
#include <MetalKit/MetalKit.hpp>
#include <memory>

class Renderer
{
    public:
        Renderer( MTL::Device* pDevice, std::unique_ptr<std::string> pShaderSrc );
        ~Renderer();

        void draw( MTK::View* pView );
        void build_shaders();
        void build_buffers();

        std::string* read_source( const char* filepath ) const;

    private:
        MTL::Device* _pDevice;
        MTL::CommandQueue* _pCmdQ;
        MTL::RenderPipelineState* _pRps;
        MTL::Library* _pLib;

        std::unique_ptr<std::string> _pShaderSrc;
};

#endif
