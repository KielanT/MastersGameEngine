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

		virtual void SetScene(std::shared_ptr<Scene> scene) = 0;

		virtual void RenderLoop() = 0;
		virtual void Renderer(Entity entity) = 0;

		virtual void Present() = 0;

		virtual void GUINewFrame() = 0;
		virtual void GUIRenderDrawData() = 0;

		virtual WindowProperties GetWindowProperties() = 0;

		virtual void ShutdownRenderer() = 0;
		
		virtual void InitGUI() = 0;
	};

	std::shared_ptr<RendererAPI> Create(const ERendererAPI API); // Create Renderer
}
