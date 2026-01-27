cbuffer CB : register(b0)
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
float4 main(InputPUV In) : SV_POSITION
{
    float output;
    output = mul(mul(viewProj, world), In.position);
    return output;
}