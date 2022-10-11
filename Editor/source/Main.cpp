#include <Engine.h>
#include "Editor.h"

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