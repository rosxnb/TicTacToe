#include <metal_stdlib>
using namespace metal;

struct v2f
{
    float4 position [[ position ]];
    half4 color;
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
    simd::float4x4 translate;
    int player;
};

static constant half4 colors[] = {
    { .9f, .9f, .9f, 1.f }, // empty tile: white
    { 0.f, 0.f, 1.f, 1.f }, // player 1: blue
    { 1.f, 0.f, 0.f, 1.f }  // player 2: red
};

vertex
v2f main_vertex( device const VertexData* vertexData [[buffer(0)]],
                 device const InstanceData* instanceData [[buffer(1)]],
                 device const WorldData& worldData [[buffer(2)]],
                 uint const vertexId [[vertex_id]],
                 uint const instanceId [[instance_id]] )
{
    v2f output;

    float4 pos = float4( vertexData[ vertexId ].position, 1.f );
    int player = instanceData[ instanceId ].player;

    output.position = worldData.proj * instanceData[ instanceId ].translate * pos;

    output.color = instanceId == 9 ? half4( 0.f, 1.f, 0.f, .4f ) : colors[player];

    return output;
}

fragment
half4 main_fragment( v2f in [[ stage_in ]], constant int* winner [[buffer(0)]] )
{
    int idx = *winner;
    return !idx ? in.color : colors[idx];
}
