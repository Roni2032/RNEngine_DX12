#include "Header.hlsli"
Texture2D<float4> texture0 : register(t0);
SamplerState sampler0 : register(s0);

float4 PSMain(OutputPUV In) : SV_Target
{
    float4 color = texture0.Sample(sampler0, In.uv);
    return color;
}