#include "Common.hlsli"


VertexOut main(VertexIn input)
{
    VertexOut output;
    
    output.PosH = mul(gViewProjectionMatrix, float4(input.PosL, 1.0f)).xyww;
    
    output.PosL = input.PosL;
    
    return output;
}