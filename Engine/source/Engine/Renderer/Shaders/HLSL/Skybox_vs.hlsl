#include "Common.hlsli"


VertexOut main(VertexIn input)
{
    VertexOut output;
    
    
    //********** DX11 book
    //**********
    
    //output.PosH = mul(float4(input.PosL, 1.0f), gViewProjectionMatrix).xyww;
    //
    //output.PosL = input.PosL;
    
    
    //********** DX12 book
    //**********
    
   output.PosL = input.PosL;
   
    float4 posW = mul(float4(input.PosL, 1.0f), gWorldMatrix);
   
   posW.xyz += gCameraPosition;
   
   output.PosH = mul(posW, gViewProjectionMatrix).xyww;
    
    return output;
}