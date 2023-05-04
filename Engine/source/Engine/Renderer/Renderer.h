#pragma once

#include "RendererAPI.h"

namespace Engine
{
	class Renderer
	{
	public:
		// Functions to call the functions on the implemented API
		static void Init(WindowProperties& props);

		static void SetScene(std::shared_ptr<Scene> scene);

		static void OnResize(int w, int height);

		static void RenderLoop();

		static void RendererEntity(Entity entity);
		static void SetSkyboxEntity(Entity entity);

		static WindowProperties GetWindowProperties();

		static void ShutdownRenderer();

		static CComPtr<ID3D11ShaderResourceView> GetSceneTexture();

		static std::shared_ptr<RendererAPI> GetRendererAPI();
		
	};
}

