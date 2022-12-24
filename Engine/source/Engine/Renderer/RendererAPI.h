#pragma once
#include <memory>
#include "Engine/SceneSystem/Scenes/Scene.h"

namespace Engine
{
	enum class ERendererAPI
	{
		None = 0,
		DirectX11
	};

	class RendererAPI
	{
	public:
		virtual bool Init(WindowProperties& props) = 0;

		virtual void RenderLoop(std::shared_ptr<Scene> scene) = 0;

		virtual void ShutdownRenderer() = 0;

	};

	std::shared_ptr<RendererAPI> Create(const ERendererAPI API); // Create Renderer
}
