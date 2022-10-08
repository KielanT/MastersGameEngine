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
			LOG_ERROR("Failed to Create Renderer");
		}
		else
		{
			m_Window = std::unique_ptr<IWindow>(IWindow::Create(props));

			if (m_Window == nullptr)
			{
				LOG_ERROR("Failed Creating Window");
			}
			
			if (!m_Renderer->InitRenderer(props)) // Initializes the renderer
			{
				LOG_ERROR("Error Initializing Renderer");
			}

			m_SceneManager = NewSceneManager(m_Renderer); // Creates the scene manager
			if (m_SceneManager == nullptr)
			{
				LOG_ERROR("Error Creating The Scene Manager");
			}
		}
	}
	
	Application::~Application()
	{
		m_SceneManager->Release();
		delete m_SceneManager; // Deletes the scene manager

		m_Renderer->ShutdownRenderer(); // Shutdown the renderer
		delete m_Renderer; // Deletes the renderer
	}
	
	void Application::Run()
	{
		m_Window->Update(m_SceneManager);
	}
	
}