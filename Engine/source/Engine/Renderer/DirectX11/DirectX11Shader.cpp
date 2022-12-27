#include "epch.h"
#include "DirectX11Shader.h"
//#include "DirectX11Renderer.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/DX11Renderer.h"

namespace Engine
{
	DirectX11Shader::~DirectX11Shader()
	{

		ReleaseShaders(); // Release the shaders
	}

	bool DirectX11Shader::InitShaders()
	{	
		std::filesystem::path currentPath = std::filesystem::current_path();

		std::filesystem::path shaderPath = currentPath;
		shaderPath.append("Shaders\\");


		std::filesystem::current_path(shaderPath);

		// Loads the vertex shaders
		//m_PixelLightingVertexShader = LoadVertexShader("PixelLighting_vs");
		/*m_BasicTransformVertexShader = LoadVertexShader("BasicTransform_vs");
		m_SkinningVertexShader = LoadVertexShader("Skinning_vs");
		m_PBRVertexShader = LoadVertexShader("PBR_vs");*/

		// Loads the pixel shaders
		/*m_LightModelPixelShader = LoadPixelShader("LightModel_ps");
		m_PixelLightingPixelShader = LoadPixelShader("PixelLighting_ps");
		m_PBRPixelShader = LoadPixelShader("PBR_ps");*/

		std::filesystem::current_path(currentPath); // Resets path to the main directory

		// CHecks that the shaders are not nullptr
		/*if (m_PixelLightingVertexShader == nullptr || m_PixelLightingPixelShader == nullptr ||
			m_BasicTransformVertexShader == nullptr || m_SkinningVertexShader == nullptr ||
			m_PBRVertexShader == nullptr || m_LightModelPixelShader == nullptr || m_PBRPixelShader == nullptr)
		{
			LOG_ERROR("Error Initializing Shaders");
			return false;
		}*/

		return true;
	}

	void DirectX11Shader::ReleaseShaders()
	{
		
	}

	CComPtr<ID3D11VertexShader> DirectX11Shader::GetVertexShader(EVertexShader vertexShader)
	{
		// Returns the selected vertex shader
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
		case EVertexShader::PBRVertexShader:
			return m_PBRVertexShader;
			break;
		}
	}

	CComPtr<ID3D11PixelShader> DirectX11Shader::GetPixelShader(EPixelShader pixelShader)
	{
		// Returns the selected pixel shader
		switch (pixelShader)
		{
		case EPixelShader::PixelLightingPixelShader:
			return m_PixelLightingPixelShader;
			break;
		case EPixelShader::LightModelPixelShader:
			return m_LightModelPixelShader;
		case EPixelShader::PBRPixelShader:
			return m_PBRPixelShader;
			break;
		}
	}

	CComPtr<ID3D11VertexShader> DirectX11Shader::LoadVertexShader(std::string shaderName)
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

	CComPtr<ID3D11PixelShader> DirectX11Shader::LoadPixelShader(std::string shaderName)
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

		/*HRESULT hr = m_Renderer->GetDevice()->CreatePixelShader(byteCode.data(), byteCode.size(), nullptr, &shader);
		if (FAILED(hr))
		{
			LOG_ERROR("Error Creating Pixel Shader");
			return nullptr;
		}*/
		
		return shader;
	}

}