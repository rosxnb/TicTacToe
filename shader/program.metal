#include <metal_stdlib>
using namespace metal;

struct v2f
{
    float4 position [[ position ]];
};

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

vertex
v2f main_vertex( device const VertexData* vertexData [[buffer(0)]],
                 device const InstanceData* instanceData [[buffer(1)]],
                 device const WorldData& worldData [[buffer(2)]],
                 uint const vertexId [[vertex_id]],
                 uint const instanceId [[instance_id]] )
{
    float4 pos = float4( vertexData[vertexId].position, 1.f );

    pos = instanceData[instanceId].translate * instanceData[instanceId].scale * pos;
    pos = worldData.proj * pos;

    v2f output;
    output.position = pos;
    return output;
}

fragment
half4 main_fragment( v2f in [[ stage_in ]] )
{
    return half4( 1.f, 0.f, 0.f, 0.f );
}
