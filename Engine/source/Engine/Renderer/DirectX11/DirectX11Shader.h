#pragma once


namespace Engine
{
	// Enum class for selecting the vertex shader
	enum class EVertexShader
	{
		PixelLightingVertexShader = 0,
		BasicTransformVertexShader,
		SkinningVertexShader,
		PBRVertexShader,

		EVertexShaderSize
	};

	// Enum class for selecting the pixel shader
	enum class EPixelShader
	{
		PixelLightingPixelShader = 0,
		LightModelPixelShader,
		PBRPixelShader,

		EPixelShaderSize
	};

	class  DirectX11Shader
	{
	public:
		~DirectX11Shader();

		// Initialize the shaders 
		bool InitShaders();

		// Release the shaders
		void ReleaseShaders();

		// Returns the selected vertex shader
		CComPtr<ID3D11VertexShader> GetVertexShader(EVertexShader vertexShader);

		// Returns the selected pixel shader
		CComPtr<ID3D11PixelShader> GetPixelShader(EPixelShader pixelShader);

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
