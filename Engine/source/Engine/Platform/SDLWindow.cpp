#include "epch.h"
#include "SDLWindow.h"

#include <sdl_syswm.h>

#include "imgui.h"
#include "backends/imgui_impl_SDL.h"
#include "backends/imgui_impl_dx11.h"

#include "Engine/Renderer/Renderer.h"
#include "Engine/Layer.h"

namespace Engine
{
	std::shared_ptr<IWindow> IWindow::Create(WindowProperties& props)
	{
		return std::make_shared<SDLWindow>(props);
	}

	SDLWindow::SDLWindow(WindowProperties& props)
	{
		Init(props);
	}
	
	SDLWindow::~SDLWindow()
	{
		Shutdown();
	}
	
	void SDLWindow::Update()
	{
		//InitInput();
		SDLInput::InitInput();

		int close = 0;

		if(ImGui::GetCurrentContext() != nullptr)
			ImGui_ImplSDL2_InitForD3D(m_Window);

		m_Timer.Start();

		while (!close)
		{

			SDL_Event event;

			// Events management
			while (SDL_PollEvent(&event))
			{
				if (ImGui::GetCurrentContext() != nullptr)
					ImGui_ImplSDL2_ProcessEvent(&event);

				if (event.type == SDL_QUIT)
				{
					close = 1;
				}
				else if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
				{
					SDLInput::KeyEvent(event.key);
				}
			}
			
			float frameTime = m_Timer.DeltaTime();
			Renderer::RenderLoop();
			Layer::Update(frameTime);
			

		}
	}
	
	void SDLWindow::Shutdown()
	{
	
		if (ImGui::GetCurrentContext() != nullptr)
		{
			
			ImGui_ImplSDL2_Shutdown();
			ImGui::DestroyContext();
		}

		SDL_DestroyWindow(m_Window);
		SDL_Quit();
	}
	
	BOOL SDLWindow::Init(WindowProperties& props)
	{
		m_Props = props;

		if (SDL_Init(SDL_INIT_EVENTS) != 0)
		{
			LOG_ERROR("Error Initializing SDL");
		}
	
		m_Window = SDL_CreateWindow(m_Props.Title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			m_Props.Width, m_Props.Height, SDL_WINDOW_RESIZABLE);
		
		SDL_SysWMinfo wmInfo;
		SDL_VERSION(&wmInfo.version);
		SDL_GetWindowWMInfo(m_Window, &wmInfo);
		m_Props.Hwnd = wmInfo.info.win.window;


		props = m_Props;

		return TRUE;
	}
}
