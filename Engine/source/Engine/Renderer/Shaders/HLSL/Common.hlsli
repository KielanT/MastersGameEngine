 //--------------------------------------------------------------------------------------
// Common include file for all shaders
//--------------------------------------------------------------------------------------
// Using include files to define the type of data passed between the shaders


//--------------------------------------------------------------------------------------
// Shader input / output
//--------------------------------------------------------------------------------------

// The structure below describes the vertex data to be sent into the vertex shader for non-skinned models
struct BasicVertex
{
    float3 position : position;
    float3 normal   : normal;
    float3 tangent : tangent;
    float2 uv       : uv;
};


struct VertexIn
{
    float3 PosL : position;
    float3 NormalL : normal;
    float2 uv : uv;
};

struct VertexOut
{
    float4 PosH : SV_Position;
    float3 PosL : position;
};

//--------------------------------------------------------------------------------------
// PBR 
//--------------------------------------------------------------------------------------


struct PBR_Input
{
    float4 projectedPosition : SV_Position;
    float3 worldPosition : worldPosition;
    float3 worldNormal : worldNormal;
    float3 tangent : tangent;
    float2 uv : uv;
};


//--------------------------------------------------------------------------------------
// Constant Buffers
//--------------------------------------------------------------------------------------

// These structures are "constant buffers" - a way of passing variables over from C++ to the GPU
// They are called constants but that only means they are constant for the duration of a single GPU draw call.
// These "constants" correspond to variables in C++ that we will change per-model, or per-frame etc.

// In this exercise the matrices used to position the camera are updated from C++ to GPU every frame along with lighting information
// These variables must match exactly the gPerFrameConstants structure in Scene.cpp
cbuffer PerFrameConstants : register(b0) // The b0 gives this constant buffer the number 0 - used in the C++ code
{
    float4x4 gViewMatrix;
    float4x4 gProjectionMatrix;
    float4x4 gViewProjectionMatrix; // The above two matrices multiplied together to combine their effects

    float3   gAmbientColour;
    float    gSpecularPower;

    float3   gCameraPosition;
    float    padding5;
}
// Note constant buffers are not structs: we don't use the name of the constant buffer, these are really just a collection of global variables (hence the 'g')


cbuffer PerModelConstants : register(b1) 
{
    float4x4 gWorldMatrix;

    float3   gObjectColour;
    float    padding6;  

}
