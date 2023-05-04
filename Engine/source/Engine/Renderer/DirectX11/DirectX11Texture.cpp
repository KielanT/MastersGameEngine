#include "epch.h"
#include "DirectX11Texture.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/DirectX11/DX11Renderer.h"


namespace Engine
{
	DirectX11Texture::DirectX11Texture(const std::string& path)
	{
		m_Path = path;

		// Create a directx 11 texture
		std::shared_ptr<DX11Renderer> dx11Render = std::static_pointer_cast<DX11Renderer>(Renderer::GetRendererAPI());
		if (dx11Render != nullptr)
		{
			bool success = dx11Render->LoadTextureFromFile(path.c_str(), &m_SRV, &m_Width, &m_Height);
			if(!success)
				LOG_ERROR("Cannot Load Texture: Texture Creation Failed");
		}
		else
		{
			LOG_ERROR("Cannot Load Texture: Renderer Access Failed");
		}
	
	}

	DirectX11Texture::~DirectX11Texture()
	{

	}
}
