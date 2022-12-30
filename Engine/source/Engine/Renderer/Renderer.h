#pragma once

#include "RendererAPI.h"

namespace Engine
{
	class Renderer
	{
	public:
		static void Init(WindowProperties& props);

		static void SetScene(std::shared_ptr<Scene> scene);

		static void RenderLoop();

		static void RendererEntity(Entity entity);

		static WindowProperties GetWindowProperties();

		static void ShutdownRenderer();

		static std::shared_ptr<RendererAPI> GetRendererAPI();
		
	};
}
