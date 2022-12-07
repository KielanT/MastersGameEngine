#include "Common.hlsli"

PBRInput main(BasicVertex input)
{
    PBRInput output;

  // Transform model vertex position and normal to world space
    float4 worldPosition = mul(float4(input.position, 1.0f), gWorldMatrix);
    output.WorldPosition = worldPosition.xyz;

	// Use camera matrices to further transform the vertex from world space into view space (camera's point of view) and finally into 2D "projection" space for rendering
    float4 viewPosition = mul(worldPosition, gViewMatrix);
    output.ProjectionPosition = mul(viewPosition, gProjectionMatrix);

  // Pass model-space normal, tangent and UVs directly to pixel shader
    output.ModelNormal = input.normal;
    output.ModelTangent = input.tangent;
    output.UV = input.uv;

    return output;
}