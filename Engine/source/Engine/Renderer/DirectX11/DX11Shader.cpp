#include "epch.h"
#include "DX11Shader.h"

#include "Engine/Renderer/Renderer.h"
#include "DX11Renderer.h"

namespace Engine
{
	bool DX11Shader::InitShaders()
	{
		std::filesystem::path currentPath = std::filesystem::current_path();

		std::filesystem::path shaderPath = currentPath;
		shaderPath.append("Shaders\\");


		std::filesystem::current_path(shaderPath);

		// Loads the vertex shaders
		m_PixelLightingVertexShader = LoadVertexShader("PixelLighting_vs");
		m_BasicTransformVertexShader = LoadVertexShader("BasicTransform_vs");
		m_SkinningVertexShader = LoadVertexShader("Skinning_vs");
		m_PBRVertexShader = LoadVertexShader("PBR_vs");
		

		// Loads the pixel shaders
		m_LightModelPixelShader = LoadPixelShader("LightModel_ps");
		m_PixelLightingPixelShader = LoadPixelShader("PixelLighting_ps");
		m_PBRPixelShader = LoadPixelShader("PBR_ps");

		std::filesystem::current_path(currentPath); // Resets path to the main 

		if (m_PixelLightingVertexShader == nullptr || m_BasicTransformVertexShader == nullptr || 
			m_SkinningVertexShader      == nullptr || m_PBRVertexShader == nullptr)
		{
			LOG_ERROR("Failed to create Vertex shaders");
			return false;
		}

		if (m_LightModelPixelShader == nullptr || m_PixelLightingPixelShader == nullptr ||
			m_PBRPixelShader == nullptr)
		{
			LOG_ERROR("Failed to create Pixel shaders");
			return false;
		}

		return true;
	}

	CComPtr<ID3D11VertexShader> DX11Shader::GetVertexShader(EVertexShader vertexShader)
	{
		switch (vertexShader)
		{
		case EVertexShader::PixelLightingVertexShader:
			return m_PixelLightingVertexShader;
			break;
		case EVertexShader::BasicTransformVertexShader:
			return m_BasicTransformVertexShader;
			break;
		case EVertexShader::SkinningVertexShader:
			return m_SkinningVertexShader;
			break;
		case EVertexShader::PBRVertexShader:
			return m_PBRVertexShader;
			break;
		}
	}

	CComPtr<ID3D11PixelShader> DX11Shader::GetPixelShader(EPixelShader pixelShader)
	{
		switch (pixelShader)
		{
		case EPixelShader::PixelLightingPixelShader:
			return m_PixelLightingPixelShader;
			break;
		case EPixelShader::LightModelPixelShader:
			return m_LightModelPixelShader;
			break;
		case EPixelShader::PBRPixelShader:
			return m_PBRPixelShader;
			break;
		}
	}

	CComPtr<ID3D11VertexShader> DX11Shader::LoadVertexShader(std::string shaderName)
	{
		// Open compiled shader object file
		std::ifstream shaderFile(shaderName + ".cso", std::ios::in | std::ios::binary | std::ios::ate);
		if (!shaderFile.is_open())
		{
			LOG_ERROR("Error Opening Shader File");
			return nullptr;
		
		}
		
		// Read file into vector of chars
		std::streamoff fileSize = shaderFile.tellg();
		shaderFile.seekg(0, std::ios::beg);
		std::vector<char> byteCode(fileSize);
		shaderFile.read(&byteCode[0], fileSize);
		if (shaderFile.fail())
		{
			LOG_ERROR("Error Reading Shader File");
			return nullptr;
		}
		
		// Create shader object from loaded file (we will use the object later when rendering)
		CComPtr<ID3D11VertexShader> shader;
		
		std::shared_ptr<DX11Renderer> render = std::static_pointer_cast<DX11Renderer>(Renderer::GetRendererAPI());
		HRESULT hr = render->GetDevice()->CreateVertexShader(byteCode.data(), byteCode.size(), nullptr, &shader);
		if (FAILED(hr))
		{
			LOG_ERROR("Error Creating Vertex Shader");
			return nullptr;
		}
		return shader;
	}

	CComPtr<ID3D11PixelShader> DX11Shader::LoadPixelShader(std::string shaderName)
	{
		// Open compiled shader object file
		std::ifstream shaderFile(shaderName + ".cso", std::ios::in | std::ios::binary | std::ios::ate);
		if (!shaderFile.is_open())
		{
			LOG_ERROR("Error Opening Shader File");
			return nullptr;
		}

		// Read file into vector of chars
		std::streamoff fileSize = shaderFile.tellg();
		shaderFile.seekg(0, std::ios::beg);
		std::vector<char>byteCode(fileSize);
		shaderFile.read(&byteCode[0], fileSize);
		if (shaderFile.fail())
		{
			LOG_ERROR("Error Reading Shader File");
			return nullptr;
		}

		// Create shader object from loaded file (we will use the object later when rendering)
		CComPtr<ID3D11PixelShader> shader;

		std::shared_ptr<DX11Renderer> render = std::static_pointer_cast<DX11Renderer>(Renderer::GetRendererAPI());
		HRESULT hr = render->GetDevice()->CreatePixelShader(byteCode.data(), byteCode.size(), nullptr, &shader);
		if (FAILED(hr))
		{
			LOG_ERROR("Error Creating Pixel Shader");
			return nullptr;
		}
		return shader;
	}


}
