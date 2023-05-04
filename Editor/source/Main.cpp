#include <Engine.h>
#include <EntryPoint.h>

#include "MainEditor.h"


class Main : public Engine::Application
{
public:
	Main()
	{
		// Adds the editor layer
		AddLayer(std::make_shared<Engine::MainEditor>());
	}
	~Main()
	{
		
	}
};

std::shared_ptr<Engine::Application> Engine::CreateApplication()
{
	return std::make_shared<Main>();
}