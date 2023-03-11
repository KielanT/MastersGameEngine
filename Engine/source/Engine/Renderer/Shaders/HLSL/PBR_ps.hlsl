
#include "Common.hlsli"

Texture2D AlbedoMap : register(t0);
Texture2D RoughnessMap : register(t1);
Texture2D NormalMap : register(t2);
Texture2D HeightMap : register(t3);
Texture2D MetalnessMap : register(t4);
TextureCube IBLMap : register(t5);

SamplerState TexSampler : register(s0);
SamplerState BilinearSampler : register(S1);


static const float GAMMA = 2.2f;
static const float PI = 3.14159265359f;

float3 SampleNormal(float3 position, float3 normal, float3 tangent, inout float2 uv, bool parallax = false)
{
    const float parallaxDepth = 0.06f;

	// Calculate inverse tangent matrix
    float3 biTangent = cross(normal, tangent);
    float3x3 invTangentMatrix = float3x3(tangent, biTangent, normal);

    if (parallax)
    {

        float3 cameraDir = normalize(gCameraPosition - position);
        float3x3 invWorldMatrix = transpose((float3x3) gWorldMatrix);
        float3 cameraModelDir = normalize(mul(cameraDir, invWorldMatrix));

    // Calculate direction to offset uvs (x and y of camera direction in tangent space)
        float3x3 tangentMatrix = transpose(invTangentMatrix);
        float2 textureOffsetDir = mul(cameraModelDir, tangentMatrix).xy;

    // Offset uvs in that direction to account for depth (using height map and some geometry)
        float texDepth = parallaxDepth * (HeightMap.Sample(TexSampler, uv).r - 0.5f);
        uv += texDepth * textureOffsetDir;
    }

	// Extract normal from map and shift to -1 to 1 range
    float3 textureNormal = 2.0f * NormalMap.Sample(TexSampler, uv).rgb - 1.0f;
    textureNormal.y = -textureNormal.y;

	// Convert normal from tangent space to world space
    return normalize(mul(mul(textureNormal, invTangentMatrix), (float3x3) gWorldMatrix));
}



float4 main(PBR_Input input) : SV_Target
{
    float3 modelNormal = normalize(input.worldNormal);
    float3 modelTangent = normalize(input.tangent);

    float3 n = SampleNormal(input.worldNormal, modelNormal, modelTangent, input.uv, true);
	
	// View vector (normal towards camera from pixel)
    float3 v = normalize(gCameraPosition - input.worldPosition);

    float3 albedo = AlbedoMap.Sample(TexSampler, input.uv).rgb;
    albedo.rgb = pow(albedo.rgb, GAMMA); // Stored in sRGB gamma-corrected space, convert to linear space
    float roughness = RoughnessMap.Sample(TexSampler, input.uv).r;
    float metalness = MetalnessMap.Sample(TexSampler, input.uv).r;
    //float cavity = lerp(1, CavityMap.Sample(TexSampler, input.uv).r, UseCavity);
    //float ao = lerp(cavity, AOMap.Sample(TexSampler, input.uv).r, UseAO);
    float nDotV = max(dot(n, v), 0.001f);
    float3 specColour = lerp(float3(0.04, 0.04, 0.04), albedo, metalness);

    
    //float3 diffuse = gAmbientColour;
    //float3 specular = gSpecularPower;
   
    float3 colour;
    if (gEnableIBL)
    {
        float3 reflectionVector = reflect(-v, n);
    
        float3 diffuseLevel = IBLMap.SampleLevel(BilinearSampler, n, 8).rgb * 2.0f;
        float3 specularLevel = IBLMap.SampleLevel(BilinearSampler, reflectionVector, 8 * log(roughness + 1) / log(2)).rgb;
    
        float3 fresnel = specColour + (1 - specColour) * pow(max(1.0f - nDotV, 0.0f), 5.0f);
        colour = /*ao * */(albedo * diffuseLevel + (1 - roughness) * fresnel * specularLevel);
    }
    else
    {
        colour = albedo * gAmbientColour;
    }
  
    
    // TODO lighting code here
    
    return float4(pow(colour, 1 / GAMMA), 1.0f);
}