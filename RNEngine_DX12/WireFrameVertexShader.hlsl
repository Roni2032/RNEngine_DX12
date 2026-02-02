cbuffer Cb0 : register(b0)
{
    matrix world;
    matrix viewProj;
    float4 color;
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

OutputPUV VSMain(InputPUV In)
{
    OutputPUV output;
    output.position = mul(mul(viewProj, world), In.position);
    output.uv = In.uv;
    return output;
}