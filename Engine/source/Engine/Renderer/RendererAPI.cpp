#include "epch.h"
#include "RendererAPI.h"

#include "DX11Renderer.h"

namespace Engine
{
	
	std::shared_ptr<RendererAPI> Create(const ERendererAPI API)
	{
		switch (API)
		{
		case ERendererAPI::None:
			return nullptr;
		case ERendererAPI::DirectX11:
			return std::make_shared<DX11Renderer>();
		}

		LOG_ERROR("API Creation Failed");
		return nullptr;
	}
}