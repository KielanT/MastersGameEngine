#include "epch.h"
#include "DirectX11Texture.h"
#include "DirectX11Renderer.h"


namespace Engine
{
	DirectX11Texture::DirectX11Texture(const std::string& path, std::shared_ptr<IRenderer> renderer)
	{
		m_Path = path;
		m_Renderer = renderer;
		
		std::shared_ptr<DirectX11Renderer> dx11Render = std::static_pointer_cast<DirectX11Renderer>(renderer);
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
