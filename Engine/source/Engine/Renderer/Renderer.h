#pragma once

#include "RendererAPI.h"

namespace Engine
{
	class Renderer
	{
	public:
		static void Init(WindowProperties& props);

		static void RenderLoop(std::shared_ptr<Scene> scene);

		static void ShutdownRenderer();

		
	};
}

