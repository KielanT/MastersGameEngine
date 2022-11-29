#include "epch.h"
#include "IRenderer.h"
#include "Engine/Renderer/DirectX11/DirectX11Renderer.h"

namespace Engine
{
	std::shared_ptr<IRenderer> NewRenderer(const ERendererType type)
	{
		switch (type)
		{
		case ERendererType::DirectX11:
			return std::make_shared<DirectX11Renderer>();
			break;
		default:
			return nullptr;
			break;
		}

	}
}