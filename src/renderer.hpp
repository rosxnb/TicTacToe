#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <Metal/Metal.hpp>
#include <MetalKit/MetalKit.hpp>
#include <memory>
#include <vector>

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
        void process_keybord_inputs();

        void draw( MTK::View* pView );
        std::vector<simd::float3> generate_line_vertices(Point2D p1, Point2D p2, float width);

        std::string read_source( const char* filepath ) const;

    private:
        MTL::Device* p_device;
        MTL::CommandQueue* p_cmdQ;
        MTL::RenderPipelineState* p_RPS;
        MTL::Library* p_shaderLibrary;

        MTL::Buffer* p_boardVertices;
        MTL::Buffer* p_boradColors;
        MTL::Buffer* p_boardIndices;

        MTL::Buffer* p_lineColors;
        MTL::Buffer* p_lineVertices;

        std::string m_shaderSrc;
};

#endif
