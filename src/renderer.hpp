#include <Metal/Metal.hpp>
#include <MetalKit/MetalKit.hpp>

class Renderer
{
    public:
        Renderer( MTL::Device* pDevice );
        ~Renderer();

        void draw( MTK::View* pView );
        void build_shaders();
        void build_buffers();

        std::string* read_source( const char* filepath ) const;

    private:
        constexpr static int maxFrames = 3;

        MTL::Device* _pDevice;
        MTL::CommandQueue* _pCmdQ;
        MTL::RenderPipelineState* _pRps;
        MTL::Library* _pLib;
        MTL::Buffer* _pArgBuff;
        MTL::Buffer* _pPositions;
        MTL::Buffer* _pColors;
        MTL::Buffer* _pIndices;
        MTL::Buffer* _pLinePoints;
        MTL::Buffer* _pLineColors;
        MTL::Buffer* _pLineArgs;

        std::string* _pShaderSrc;
};
