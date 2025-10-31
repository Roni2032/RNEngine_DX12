cbuffer Cb0 : register(b0)
{
    matrix world;
    matrix viewProj;
}

struct InputPUV
{
    float4 position : POSITION;
    float2 uv : TEXCOORD0;
};
struct OutputPUV
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};