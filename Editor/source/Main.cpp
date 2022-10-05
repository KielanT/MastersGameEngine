#include <Engine.h>

class Main : public Engine::Application
{
public:
	Main()
	{
		
	}
	~Main()
	{
		
	}
};

Engine::Application* Engine::CreateApplication()
{
	return new Main();
}