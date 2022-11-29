#pragma once

#include "Engine/Utility/Logger.h"

#ifdef E_PLATFORM_WINDOWS

extern std::shared_ptr<Engine::Application> Engine::CreateApplication();

// The main entry point for the engine
int main(int argc, char** argv)
{
	Engine::Logger::Init();

	auto app = Engine::CreateApplication();
	app->Run();

	return 0;
}

#endif // E_PLATFORM_WINDOWS

