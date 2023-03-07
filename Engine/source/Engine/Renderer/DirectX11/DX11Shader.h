#pragma once

#include "Engine/Scene/Components.h"

namespace Engine
{

	class DX11Shader
	{
	public:
		bool InitShaders();

		CComPtr<ID3D11VertexShader> GetVertexShader();

		CComPtr<ID3D11PixelShader> GetPixelShader();

	private:
		CComPtr<ID3D11VertexShader> LoadVertexShader(std::string shaderName); // Loads the vertex shader
		CComPtr<ID3D11PixelShader> LoadPixelShader(std::string shaderName); // Loads the pixel shader

	private:
		// Vertex shader member variables
		CComPtr<ID3D11VertexShader> m_PBRVertexShader;

		// Pixel shader member variables
		CComPtr<ID3D11PixelShader> m_PBRPixelShader;
	};
}
