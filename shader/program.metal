#include <metal_stdlib>
using namespace metal;

struct V2F
{
    float4 position [[ position ]];
    half3 color;
};

struct VertexData
{
    device float3* position [[ id(0) ]];
    device float3* color [[ id(1) ]];
};

vertex
V2F main_vertex( device const float3* vertices [[buffer(0)]],
                 device const float3* colors [[buffer(1)]],
                 uint const id [[vertex_id]] )
{
    V2F retval;
    retval.position = float4( vertices[id], 1.f );
    retval.color = half3( colors[0] );
    return retval;
}

fragment
half4 main_fragment( V2F data [[ stage_in ]] )
{
    return half4( data.color, 1.f );
}
