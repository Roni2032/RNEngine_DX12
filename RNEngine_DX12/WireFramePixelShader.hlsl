cbuffer Cb0 : register(b0)
{
    matrix world;
    matrix viewProj;
    float4 color;
}

struct OutputPUV
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

float4 PSMain(OutputPUV In) : SV_TARGET
{
    return color;

}