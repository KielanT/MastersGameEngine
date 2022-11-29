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

std::shared_ptr<Engine::Application> Engine::CreateApplication()
{
	return std::make_shared<Main>();
}