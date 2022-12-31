#include <Engine.h>
#include <EntryPoint.h>
#include "Game.h"


class Main : public Engine::Application
{
public:
	Main()
	{
		AddLayer(std::make_shared<Engine::Game>());
	}
	~Main()
	{
		
	}
};

std::shared_ptr<Engine::Application> Engine::CreateApplication()
{
	return std::make_shared<Main>();
}