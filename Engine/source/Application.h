#pragma once

#include "Engine/Platform/WindowsWindow.h"

#include <memory>

namespace Engine
{
	class Application
	{
	public:
		Application();
		~Application();

		Application(const Application&) = delete;
		Application(Application&&) = delete;
		Application& operator=(const Application&) = delete;
		Application& operator=(Application&&) = delete;
		

	public:
		void Run();

	private:
		std::unique_ptr<IWindow> m_Window;
	};

	Application* CreateApplication();
}

