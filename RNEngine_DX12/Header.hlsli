cbuffer Cb0 : register(b0)
{
    matrix world;
    matrix viewProj;
}

struct VertexInput
{
    float4 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
    float4 boneWeight : BONEWEIGHTS;
    uint4 boneIndex : BONEINDICES;
};
struct OutputPUV
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};