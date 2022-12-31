#include <Engine.h>
#include <EntryPoint.h>
#include "Editor.h"


class Main : public Engine::Application
{
public:
	Main()
	{
		AddLayer(std::make_shared<Engine::Editor>());
	}
	~Main()
	{
		
	}
};

std::shared_ptr<Engine::Application> Engine::CreateApplication()
{
	return std::make_shared<Main>();
}