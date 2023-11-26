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
        static constexpr float gutter = 25.f;
        static constexpr float tileWidth = (window_width - 4.f * gutter) / 3.f;
        static constexpr float tileHeight = (window_height - 4.f * gutter) / 3.f;

        MTL::Buffer* p_vertices = nullptr;
        MTL::Buffer* p_indices = nullptr;
        MTL::Buffer* p_instances[kMaxFrames] = {nullptr};
        MTL::Buffer* p_projection = nullptr;

        uint m_frame = 0;
        std::string m_shaderSrc;
        dispatch_semaphore_t m_semaphore;

        int m_winner = 0;
        std::array<int, kNumInstances> m_moves;

        double h_curr_row = 1;
        double h_curr_col = 1;

        int m_player = 1;

    private:
        constexpr float get_x_translate_value(uint tileNo) { return ( (float) tileNo * gutter + (float) (tileNo - 1) * tileWidth ); }
        constexpr float get_y_translate_value(uint tileNo) { return ( (float) tileNo * gutter + (float) (tileNo - 1) * tileHeight ); }

        std::pair<size_t, size_t> process_keyboard_input();
        size_t get_selected_tile();
        bool tictactoe_terminal_state() const;
        bool tictactoe_has_winner() const;
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
        simd::float4x4 translate;
        int player;
    };

}

#endif
