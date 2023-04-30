#include "Common.hlsli"


PBR_Input main(BasicVertex input)
{
    PBR_Input output;

    float4 worldPosition = mul(gWorldMatrix, float4(input.position, 1.0f));
    output.worldPosition = worldPosition.xyz;
 
    float4 viewPosition = mul(gViewMatrix, worldPosition);
    output.projectedPosition = mul(gProjectionMatrix, viewPosition);
 
    float4 modelNormal = float4(input.normal, 0);
    output.worldNormal = mul(modelNormal, gWorldMatrix).xyz;
    output.tangent = input.tangent;
    output.uv = input.uv;
  
    return output;

}