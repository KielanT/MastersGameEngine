#pragma once

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
		
	};

	Application* CreateApplication();
}

