#include "epch.h"
#include "Application.h"

#include <SDL.h>
#include "Engine/Renderer/Renderer.h"

namespace Engine
{
	Application::Application()
	{
		WindowProperties props = WindowProperties();

		m_Window = IWindow::Create(props);

		if (m_Window == nullptr)
		{
			LOG_ERROR("Failed Creating Window");
		}

		Renderer::Init(props);
	}
	
	Application::~Application()
	{
		Renderer::ShutdownRenderer();

	}
	
	void Application::Run()
	{
		m_Window->Update();
	}


	
}