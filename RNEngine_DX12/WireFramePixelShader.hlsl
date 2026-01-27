cbuffer CB : register(b0)
{
    matrix world;
    matrix viewProj;
    float4 color;
}

float4 main() : SV_TARGET
{
    return color;
}