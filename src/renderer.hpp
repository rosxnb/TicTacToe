#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <Metal/Metal.hpp>
#include <MetalKit/MetalKit.hpp>
#include <memory>
#include <vector>

#include "Metal/MTLIntersectionFunctionTable.hpp"
#include "utility.hpp"
#include "simd/simd.h"

constexpr float window_width  = 1000.f;
constexpr float window_height = 1000.f;

class Renderer
{
    public:
        Renderer( MTL::Device* pDevice );
        ~Renderer();

        void build_shaders();
        void build_buffers();
        void draw( MTK::View* pView );

        std::string read_source( const char* filepath ) const;

    private:
        MTL::Device* p_device = nullptr;
        MTL::CommandQueue* p_cmdQueue = nullptr;
        MTL::RenderPipelineState* p_renderPipeline = nullptr;
        MTL::Library* p_shaderLibrary = nullptr;

        static constexpr size_t kNumInstances = 9;
        static constexpr size_t kMaxFrames = 3;
        static constexpr float tileWidth = 300.f;
        static constexpr float tileHeight = 300.f;
        static constexpr float gutter = 25.f;

        MTL::Buffer* p_vertices = nullptr;
        MTL::Buffer* p_indices = nullptr;
        MTL::Buffer* p_instances[kMaxFrames] = {nullptr};
        MTL::Buffer* p_projection = nullptr;

        uint m_frame = 0;
        std::string m_shaderSrc;
        dispatch_semaphore_t m_semaphore;

    private:
        constexpr float get_x_translate_value(uint tileNo) { return ( (float) tileNo * gutter + (float) (tileNo - 1) * tileWidth ); }
        constexpr float get_y_translate_value(uint tileNo) { return ( (float) tileNo * gutter + (float) (tileNo - 1) * tileHeight ); }

        void process_keybord_inputs();
};

namespace shader_types
{

    struct VertexData
    {
        simd::float3 position;
    };

    struct WorldData
    {
        simd::float4x4 proj;
    };

    struct InstanceData
    {
        simd::float4x4 scale;
        simd::float4x4 translate;
    };

}

#endif
