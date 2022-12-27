#include "epch.h"
#include "IShader.h"
#include "Engine/Renderer/DirectX11/DirectX11Shader.h"

namespace Engine
{
	std::shared_ptr<IShader> SetShader(ERendererType renderertype)
	{
		if (renderertype == ERendererType::DirectX11) // Returns the DirectX 11 shaders
		{
			return nullptr;// std::make_shared<DirectX11Shader>();
		}
		else
		{
			return nullptr;
		}
	}
}