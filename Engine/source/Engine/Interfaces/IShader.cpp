#include "epch.h"
#include "IShader.h"
#include "Engine/Renderer/DirectX11/DirectX11Shader.h"

namespace Engine
{
	IShader* SetShader(ERendererType renderertype)
	{
		if (renderertype == ERendererType::DirectX11) // Returns the DirectX 11 shaders
		{
			return new DirectX11Shader();
		}
		else
		{
			return nullptr;
		}
	}
}