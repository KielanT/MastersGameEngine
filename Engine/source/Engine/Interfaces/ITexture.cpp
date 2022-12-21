#include "epch.h"

#include "ITexture.h"
#include "Engine/Renderer/DirectX11/DirectX11Texture.h"

namespace Engine
{
	std::shared_ptr<Texture2D> Texture2D::Create(const std::string& path, std::shared_ptr<IRenderer> renderer)
	{
		return std::make_shared<DirectX11Texture>(path, renderer);
	}
}