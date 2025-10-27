#include "Header.hlsli"

OutputPUV VSMain(InputPUV In)
{
    OutputPUV output;
    output.position = In.position;
    output.uv = In.uv;
    return output;
}