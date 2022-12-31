#pragma once

#include "Engine/Utility/Logger.h"

#ifdef E_PLATFORM_WINDOWS

extern std::shared_ptr<Engine::Application> Engine::CreateApplication();

// The main entry point for the engine

#ifdef E_EDITOR 
int main(int argc, char** argv)
{
	Engine::Logger::Init();

	auto app = Engine::CreateApplication();
	app->Run();

	return 0;
}
#endif

#ifndef E_EDITOR // If Game is start up this will still work, the IDE just shows otherwise
int WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{
	Engine::Logger::Init();

	auto app = Engine::CreateApplication();
	app->Run();

	return 0;
}
#endif // !1

#endif // E_PLATFORM_WINDOWS

