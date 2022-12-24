#include "epch.h"
#include "Renderer.h"


namespace Engine
{
	static std::shared_ptr<RendererAPI> m_RendererAPI = nullptr;

	void Renderer::Init(WindowProperties& props)
	{
		// Create Renderer based on the API selected
		m_RendererAPI = Create(ERendererAPI::DirectX11);
		m_RendererAPI->Init(props);
		LOG_DEBUG("Renderer Initilised");
	}

	void Renderer::RenderLoop(std::shared_ptr<Scene> scene)
	{
		m_RendererAPI->RenderLoop(scene);
	}

	void Renderer::ShutdownRenderer()
	{
		m_RendererAPI->ShutdownRenderer();
		LOG_DEBUG("Renderer shutdown");
	}

	

}