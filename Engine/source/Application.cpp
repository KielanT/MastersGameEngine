#include "epch.h"
#include "Application.h"

#include <SDL.h>
#include "Engine/Renderer/Renderer.h"

namespace Engine
{
	Application::Application()
	{
		WindowProperties props = WindowProperties();

		

		m_Renderer = NewRenderer(props.RendererType);
		if (m_Renderer == nullptr)
		{
			LOG_ERROR("Failed to Create Renderer");
		}
		else
		{
			m_Window = IWindow::Create(props);

			if (m_Window == nullptr)
			{
				LOG_ERROR("Failed Creating Window");
			}
			
			if (!m_Renderer->InitRenderer(props)) // Initializes the renderer
			{
				LOG_ERROR("Error Initializing Renderer");
			}

			Renderer::Init(props);

			m_SceneManager = NewSceneManager(m_Renderer); // Creates the scene manager
			if (m_SceneManager == nullptr)
			{
				LOG_ERROR("Error Creating The Scene Manager");
			}
		}
	}
	
	Application::~Application()
	{
		Renderer::ShutdownRenderer();
		m_SceneManager->Release();

		m_Renderer->ShutdownRenderer(); // Shutdown the renderer
	}
	
	void Application::Run()
	{
		m_Window->Update(m_SceneManager);
	}


	
}