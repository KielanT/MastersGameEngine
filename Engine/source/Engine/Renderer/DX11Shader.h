#pragma once

namespace Engine
{
	enum class EDX11VertexShader
	{
		PixelLightingVertexShader = 0,
		BasicTransformVertexShader,
		SkinningVertexShader,
		PBRVertexShader,

		EVertexShaderSize
	};

	enum class EDX11PixelShader
	{
		PixelLightingPixelShader = 0,
		LightModelPixelShader,
		PBRPixelShader,

		EPixelShaderSize
	};

	class DX11Shader
	{
	public:
		bool InitShaders();

		CComPtr<ID3D11VertexShader> GetVertexShader(EDX11VertexShader vertexShader);

		CComPtr<ID3D11PixelShader> GetPixelShader(EDX11PixelShader pixelShader);

	private:
		CComPtr<ID3D11VertexShader> LoadVertexShader(std::string shaderName); // Loads the vertex shader
		CComPtr<ID3D11PixelShader> LoadPixelShader(std::string shaderName); // Loads the pixel shader

	private:
		// Vertex shader member variables
		CComPtr<ID3D11VertexShader> m_PixelLightingVertexShader;
		CComPtr<ID3D11VertexShader> m_BasicTransformVertexShader;
		CComPtr<ID3D11VertexShader> m_SkinningVertexShader;
		CComPtr<ID3D11VertexShader> m_PBRVertexShader;

		// Pixel shader member variables
		CComPtr<ID3D11PixelShader> m_LightModelPixelShader;
		CComPtr<ID3D11PixelShader> m_PixelLightingPixelShader;
		CComPtr<ID3D11PixelShader> m_PBRPixelShader;
	};
}
