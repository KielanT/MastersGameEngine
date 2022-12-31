#pragma once

#include "Engine/Interfaces/IWindow.h"
#include "Engine/Interfaces/ILayer.h"



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
		void AddLayer(std::shared_ptr<ILayer> layer) { m_Layer = layer; }

		
	private:
		std::shared_ptr<IWindow> m_Window;
		std::shared_ptr<ILayer> m_Layer;

	};

	std::shared_ptr<Application> CreateApplication();
}

