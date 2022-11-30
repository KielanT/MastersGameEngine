#pragma once


#include "Engine/Interfaces/IShader.h"

namespace Engine
{
	// Enum class for selecting the vertex shader
	enum class EVertexShader
	{
		PixelLightingVertexShader = 0,
		BasicTransformVertexShader,
		SkinningVertexShader,

		EVertexShaderSize
	};

	// Enum class for selecting the pixel shader
	enum class EPixelShader
	{
		PixelLightingPixelShader = 0,
		LightModelPixelShader,

		EPixelShaderSize
	};

	class  DirectX11Shader : public IShader
	{
	public:
		~DirectX11Shader();

		// Initialize the shaders 
		virtual bool InitShaders(std::shared_ptr<IRenderer> renderer) override;

		// Release the shaders
		virtual void ReleaseShaders() override;

		// Returns the selected vertex shader
		CComPtr<ID3D11VertexShader> GetVertexShader(EVertexShader vertexShader);

		// Returns the selected pixel shader
		CComPtr<ID3D11PixelShader> GetPixelShader(EPixelShader pixelShader);

	private:

		CComPtr<ID3D11VertexShader> LoadVertexShader(std::string shaderName); // Loads the vertex shader
		CComPtr<ID3D11PixelShader> LoadPixelShader(std::string shaderName); // Loads the pixel shader

	private:
		std::shared_ptr<IRenderer> m_Renderer; // Used for getting the renderer in use

		// Vertex shader member variables
		CComPtr<ID3D11VertexShader> m_PixelLightingVertexShader;
		CComPtr<ID3D11VertexShader> m_BasicTransformVertexShader;
		CComPtr<ID3D11VertexShader> m_SkinningVertexShader;

		// Pixel shader member variables
		CComPtr<ID3D11PixelShader> m_LightModelPixelShader;
		CComPtr<ID3D11PixelShader> m_PixelLightingPixelShader;

	};
}
