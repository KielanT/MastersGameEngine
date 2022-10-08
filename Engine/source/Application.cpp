#include "epch.h"
#include "Application.h"

namespace Engine
{
	Application::Application()
	{
		WindowProperties props = WindowProperties();
		

		m_Renderer = NewRenderer(props.RendererType);
		if (m_Renderer == nullptr)
		{
			//ENGINE_ERROR("Failed to create renderer");
		}
		else
		{
			m_Window = std::unique_ptr<IWindow>(IWindow::Create(props));

			if (m_Window == nullptr)
			{
				// Error
			}
			
			if (!m_Renderer->Init(props)) // Initializes the renderer
			{
				//LOG_ERROR("Error Initializing the renderer");
			}

			m_SceneManager = NewSceneManager(m_Renderer); // Creates the scene manager
			if (m_SceneManager == nullptr)
			{
				//LOG_ERROR("Error Creating the scene manager");
			}
		}
	}
	
	Application::~Application()
	{
		m_SceneManager->Release();
		delete m_SceneManager; // Deletes the scene manager

		m_Renderer->Shutdown(); // Shutdown the renderer
		delete m_Renderer; // Deletes the renderer
	}
	
	void Application::Run()
	{
		m_Window->Update();
	}
	
}