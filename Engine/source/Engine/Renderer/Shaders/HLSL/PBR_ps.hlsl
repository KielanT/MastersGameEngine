
#include "Common.hlsli"

Texture2D AlbedoMap : register(t0);
Texture2D RoughnessMap : register(t2);
Texture2D NormalMap : register(t3);
Texture2D HeightMap : register(t4);
Texture2D MetalnessMap : register(t5);

SamplerState TexSampler : register(s0);


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
    float3 specColour = lerp(float3(0.04, 0.04, 0.04), albedo, metalness);

    
    float3 diffuse = gAmbientColour;
    float3 specular = gSpecularPower;
    float3 colour = albedo * gAmbientColour;;// * ao;
   
    //for (int i = 0; i < NUM_LIGHTS; ++i)
    //{
 
    //    float3 l = Lights[i].position - input.WorldPosition;
    //    float rdist = 1 / length(l);
    //    l *= rdist;
    //    float li = Lights[i].intensity * rdist * rdist;
    //    float3 lc = Lights[i].colour;

    //    float3 h = normalize(l + v);

    //    diffuse += li * lc * max(dot(n, l), 0);
    //    specular += li * lc * pow(max(dot(n, h), 0), SpecularPower);

    //    float nv = dot(n, v);
    //    if (nv < 0.0f)
    //        nv = 0.001f;

    //    float nl = dot(n, l);
    //    if (nl < 0.0f)
    //        nl = 0.001f;

    //    float nh = dot(n, h);
    //    if (nh < 0.0f)
    //        nh = 0.001f;

    //    float3 Lambert = albedo / PI;
       
    //    float3 roughnessSquare = (roughness * roughness) * (roughness * roughness);
    //    float3 nhSquare = nh * nh;
    //    float3 normalDistribution = roughnessSquare / (PI * pow(nhSquare * (roughnessSquare - 1) + 1, 2));
        
    //    float k = (roughness + 1) * (roughness + 1) / 8;
    //    float3 geom = nl / (nl * (1 - k) + k);
    //    float3 geometry = nv / (nv * (1 - k) + k);
    //    float3 finalGeom = geometry * geom;
        
    //    float3 fresnel = specColour + (1 - specColour) * pow(1 - nh, 5);
    //    float3 specularBRDF = (fresnel * finalGeom * normalDistribution) / (4 * nl * nv);
        
        
    //    float3 punctualLight = Lambert + specularBRDF;
    //    colour += PI * punctualLight * li * lc * nl * cavity;
    //}

    return float4(pow(colour, 1 / GAMMA), 1.0f);
}