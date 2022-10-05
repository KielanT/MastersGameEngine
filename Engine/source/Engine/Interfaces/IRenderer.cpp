#include "epch.h"
#include "IRenderer.h"
#include "Engine/Renderer/DirectX11/DirectX11Renderer.h"

namespace Engine
{
	IRenderer* NewRenderer(const ERendererType type)
	{
		switch (type)
		{
		case ERendererType::DirectX11:
			return new DirectX11Renderer();
			break;
		default:
			return nullptr;
			break;
		}

	}
}