#pragma once

#include "Engine/Interfaces/IWindow.h"
#include "Engine/Interfaces/IRenderer.h"
#include "Engine/Interfaces/ISceneManager.h"


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
		std::shared_ptr<IWindow> m_Window;
		std::shared_ptr<IRenderer> m_Renderer;

		std::shared_ptr<ISceneManager> m_SceneManager;

	};

	std::shared_ptr<Application> CreateApplication();
}

