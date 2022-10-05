#include "epch.h"
#include "Application.h"

namespace Engine
{
	Application::Application()
	{
		WindowProperties props = WindowProperties();
		m_Window = std::unique_ptr<IWindow>(IWindow::Create(props));
		if (m_Window == nullptr)
		{
			// Error
		}
	}
	
	Application::~Application()
	{
	}
	
	void Application::Run()
	{
		m_Window->Update();
	}
	
}