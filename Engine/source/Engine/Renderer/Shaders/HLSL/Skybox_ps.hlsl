#include "Common.hlsli"


TextureCube CubeMap : register(t0);
SamplerState TexSampler : register(s0);

float4 main(VertexOut input) : SV_Target
{
	
    return CubeMap.Sample(TexSampler, input.PosL);
}