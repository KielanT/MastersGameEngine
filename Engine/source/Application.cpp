#include "epch.h"
#include "Application.h"
#include "Engine/Layer.h"

#include <SDL.h>
#include "Engine/Renderer/Renderer.h"
#include "Engine/Scripting/Scripting.h"

namespace Engine
{
	// Used for creating the engine application
	Application::Application()
	{
		WindowProperties props = WindowProperties();

		// Create window
		m_Window = IWindow::Create(props);

		if (m_Window == nullptr)
		{
			LOG_ERROR("Failed Creating Window");
		}

		// Create renderer
		Renderer::Init(props);
		
		
		// Create script
		if (!Scripting::GetInstance()->InitScripting())
		{
			LOG_ERROR("Failed Creating Scripting Engine");
		}
	}
	
	Application::~Application()
	{
		// Shutdown 
		Renderer::ShutdownRenderer();
		Scripting::GetInstance()->ShutdownScripting();

	}
	
	void Application::Run()
	{
		// Create the layer and then run the application
		Layer::Init(m_Layer);
		m_Window->Update();
	}


	
}