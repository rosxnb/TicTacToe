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
V2F main_vertex( device const VertexData* vertexData [[ buffer(0) ]],
                 uint const vertexId [[ vertex_id ]] )
{
    V2F out;
    out.position = float4( vertexData->position[ vertexId ], 1.f );
    out.color = half3( vertexData->color[ vertexId ] );
    return out;
}

fragment
half4 main_fragment( V2F data [[ stage_in ]] )
{
    return half4( data.color, 1.f );
}
