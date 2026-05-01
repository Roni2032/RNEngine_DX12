#include "Header.hlsli"

OutputPUV VSMain(VertexInput In)
{
    OutputPUV output;
    output.position = mul(mul(viewProj, world), In.position);
    output.uv = In.uv;
    return output;
}