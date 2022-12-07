
#include "Common.hlsli"

Texture2D AlbedoMap : register(t0);
Texture2D RoughnessMap : register(t1);
Texture2D NormalMap : register(t2);
Texture2D HeightMap : register(t3);
Texture2D MetalnessMap : register(t4);

// TODO: Add Optional Maps
// Optional
//Texture2D AOMap; 
//Texture2D CavityMap;

SamplerState TexSampler : register(s0);

// Constants
static const float GAMMA = 2.2f;
static const float PI = 3.14159265359f;

float3 SampleNormal(float3 position, float3 normal, float3 tangant, inout float2 UV, bool parallax = false)
{
    const float parallaxDepth = 0.06f;
	
    float3 biTangent = cross(normal, tangant);
	
    float3x3 invTangantMatrix = float3x3(tangant, biTangent, normal);
	
    if (parallax)
    {
        float3 cameraDirection = normalize(gCameraPosition - position);
        float3x3 invWorldMatrix = transpose((float3x3) gWorldMatrix);
        float3 cameraModelDir = normalize(mul(cameraDirection, invWorldMatrix));

        float3x3 tangantMatrix = transpose(invTangantMatrix);
        float2 textureOffsetDir = mul(cameraModelDir, tangantMatrix).xy;
        
        float texDepth = parallaxDepth * (HeightMap.Sample(TexSampler, UV).r - 0.5f);
        UV += texDepth * textureOffsetDir;
    }
    
    float3 textureNormal = 2.0f * NormalMap.Sample(TexSampler, UV).rgb - 1.0f;
    textureNormal.y = -textureNormal.y;
   
    return normalize(mul(mul(textureNormal, invTangantMatrix), (float3x3) gWorldMatrix));

}

float4 main(PBRInput input) : SV_Target
{
    float3 modelNormal = normalize(input.ModelNormal);
    float3 modelTangent = normalize(input.ModelTangent);

    float3 n = SampleNormal(input.WorldPosition, modelNormal, modelTangent, input.UV, true);

    float3 v = normalize(gCameraPosition - input.WorldPosition);

    float3 albedo = AlbedoMap.Sample(TexSampler, input.UV).rgb;
    albedo.rgb = pow(albedo.rgb, GAMMA); 
    float roughness = RoughnessMap.Sample(TexSampler, input.UV).r;
    float metalness = MetalnessMap.Sample(TexSampler, input.UV).r;

    float3 specColour = lerp(float3(0.04, 0.04, 0.04), albedo, metalness);
    float3 diffuse = gAmbientColour;
    float3 specular = 0;
    float3 colour = albedo * gAmbientColour /** ao*/;
    for (int i = 0; i < 2; ++i)
    {
        float3 l = float3(0, 50, 0)/*Lights[i].position*/ - input.WorldPosition;
        float rdist = 1 / length(l);
        l *= rdist;
        float li = /*Lights[i].intensity*/ 50 * rdist * rdist;
        float3 lc = float3(1.0f, 1.0f, 0.0f) /*Lights[i].colour*/;

        float3 h = normalize(l + v);

    
        diffuse += li * lc * max(dot(n, l), 0);
        specular += li * lc * pow(max(dot(n, h), 0), gSpecularPower);

        
        float nv = dot(n, v);
        if (nv < 0.0f)
            nv = 0.001f;

        float nl = dot(n, l);
        if (nl < 0.0f)
            nl = 0.001f;

        float nh = dot(n, h);
        if (nh < 0.0f)
            nh = 0.001f;

		
        float3 Lambert = albedo / PI;
        
        float3 roughnessSquare = (roughness * roughness) * (roughness * roughness);
        float3 nhSquare = nh * nh;
        float3 normalDistribution = roughnessSquare / (PI * pow(nhSquare * (roughnessSquare - 1) + 1, 2));
        
        float k = (roughness + 1) * (roughness + 1) / 8;
        float3 geom = nl / (nl * (1 - k) + k);
        float3 geometry = nv / (nv * (1 - k) + k);
        float3 finalGeom = geometry * geom;
        
        float3 fresnel = specColour + (1 - specColour) * pow(1 - nh, 5);
        float3 specularBRDF = (fresnel * finalGeom * normalDistribution) / (4 * nl * nv);
        
        
        float3 punctualLight = Lambert + specularBRDF;
        colour += PI * punctualLight * li * lc * nl /** cavity*/;
    }

    return float4(pow(colour, 1 / GAMMA), 1.0f);
}