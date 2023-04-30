#include "epch.h"
#include "DX11Shader.h"

#include "Engine/Renderer/Renderer.h"
#include "DX11Renderer.h"

namespace Engine
{
	bool DX11Shader::InitShaders()
	{
		std::filesystem::path currentPath = std::filesystem::current_path();

		std::filesystem::path shaderPath = currentPath.parent_path();
		shaderPath.append("Engine\\Shaders\\");


		std::filesystem::current_path(shaderPath);

		// Loads the vertex shader
		m_PBRVertexShader = LoadVertexShader("PBR_vs");
		m_SkyboxVertexShader = LoadVertexShader("Skybox_vs");

		// Loads the pixel shader
		m_PBRPixelShader = LoadPixelShader("PBR_ps");
		m_SkyboxPixelShader = LoadPixelShader("Skybox_ps");

		std::filesystem::current_path(currentPath); // Resets path to the main 

		if (m_PBRVertexShader == nullptr || m_SkyboxVertexShader == nullptr)
		{
			LOG_ERROR("Failed to create Vertex shader");
			return false;
		}

		if (m_PBRPixelShader == nullptr || m_SkyboxPixelShader == nullptr)
		{
			LOG_ERROR("Failed to create Pixel shader");
			return false;
		}

		return true;
	}

	CComPtr<ID3D11VertexShader> DX11Shader::GetVertexShader(EShaderType type)
	{
		switch (type)
		{
		case Engine::EShaderType::PBR:
			return m_PBRVertexShader;
			break;
		case Engine::EShaderType::Skybox:
			return m_SkyboxVertexShader;
			break;
		default:
			return m_PBRVertexShader;
			break;
		}

	}

	CComPtr<ID3D11PixelShader> DX11Shader::GetPixelShader(EShaderType type)
	{
		switch (type)
		{
		case Engine::EShaderType::PBR:
			return m_PBRPixelShader;
			break;
		case Engine::EShaderType::Skybox:
			return m_SkyboxPixelShader;
			break;
		default:
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
