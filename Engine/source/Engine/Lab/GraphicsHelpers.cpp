//--------------------------------------------------------------------------------------
// Helper functions to unclutter and simplify code elsewhere
//--------------------------------------------------------------------------------------
#include "epch.h"
#include "GraphicsHelpers.h"




//--------------------------------------------------------------------------------------
// Camera Helpers
//--------------------------------------------------------------------------------------

// A "Engineion matrix" contains properties of a camera. Covered mid-module - the maths is an optional topic (not examinable).
// - Aspect ratio is screen width / height (like 4:3, 16:9)
// - FOVx is the viewing angle from left->right (high values give a fish-eye look),
// - near and far clip are the range of z distances that can be rendered
glm::mat4 MakeEngineionMatrix(float aspectRatio /*= 4.0f / 3.0f*/, float FOVx /*= ToRadians(60)*/,
                                float nearClip /*= 0.1f*/, float farClip /*= 10000.0f*/)
{
    float tanFOVx = std::tan(FOVx * 0.5f);
    float scaleX = 1.0f / tanFOVx;
    float scaleY = aspectRatio / tanFOVx;
    float scaleZa = farClip / (farClip - nearClip);
    float scaleZb = -nearClip * scaleZa;

    return glm::mat4{ scaleX,   0.0f,    0.0f,   0.0f,
                         0.0f, scaleY,    0.0f,   0.0f,
                         0.0f,   0.0f, scaleZa,   1.0f,
                         0.0f,   0.0f, scaleZb,   0.0f };
}

ID3DBlob* CreateSignatureForVertexLayout(const D3D11_INPUT_ELEMENT_DESC vertexLayout[], int numElements)
{
    std::string shaderSource = "float4 main(";
    for (int elt = 0; elt < numElements; ++elt)
    {
        auto& format = vertexLayout[elt].Format;
        // This list should be more complete for production use
        if (format == DXGI_FORMAT_R32G32B32A32_FLOAT) shaderSource += "float4";
        else if (format == DXGI_FORMAT_R32G32B32_FLOAT)    shaderSource += "float3";
        else if (format == DXGI_FORMAT_R32G32_FLOAT)       shaderSource += "float2";
        else if (format == DXGI_FORMAT_R32_FLOAT)          shaderSource += "float";
        else if (format == DXGI_FORMAT_R8G8B8A8_UINT)      shaderSource += "uint4";
        else return nullptr; // Unsupported type in layout

        uint8_t index = static_cast<uint8_t>(vertexLayout[elt].SemanticIndex);
        std::string semanticName = vertexLayout[elt].SemanticName;
        semanticName += ('0' + index);

        shaderSource += " ";
        shaderSource += semanticName;
        shaderSource += " : ";
        shaderSource += semanticName;
        if (elt != numElements - 1)  shaderSource += " , ";
    }
    shaderSource += ") : SV_Position {return 0;}";

    ID3DBlob* compiledShader;
    HRESULT hr = D3DCompile(shaderSource.c_str(), shaderSource.length(), NULL, NULL, NULL, "main",
        "vs_5_0", D3DCOMPILE_OPTIMIZATION_LEVEL0, 0, &compiledShader, NULL);
    if (FAILED(hr))
    {
        return nullptr;
    }

    return compiledShader;
}
