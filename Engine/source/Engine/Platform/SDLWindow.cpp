#include "epch.h"
#include "SDLWindow.h"

#include <sdl_syswm.h>

#include "imgui.h"
#include "backends/imgui_impl_SDL.h"
#include "backends/imgui_impl_dx11.h"

#include "Engine/Renderer/DirectX11/DirectX11Renderer.h"



namespace Engine
{
	IWindow* IWindow::Create(WindowProperties& props)
	{
		return new SDLWindow(props);
	}

	SDLWindow::SDLWindow(WindowProperties& props)
	{
		Init(props);
	}
	
	SDLWindow::~SDLWindow()
	{
		Shutdown();
	}
	
	void SDLWindow::Update(ISceneManager* m_SceneManager)
	{
		//InitInput();
		SDLInput::InitInput();

		int close = 0;

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		ImGui_ImplSDL2_InitForD3D(m_Window);

		DirectX11Renderer* renderer = static_cast<DirectX11Renderer*>(m_SceneManager->GetRenderer());
		ImGui_ImplDX11_Init(renderer->GetDevice(), renderer->GetDeviceContext());

		// Initialise scene
		if (!m_SceneManager->LoadFirstScene())
		{
			LOG_ERROR("Error Loading First Scene");
		}
		m_Timer.Start();

		while (!close)
		{

			SDL_Event event;

			// Events management
			while (SDL_PollEvent(&event))
			{
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
			m_SceneManager->SceneLoop(frameTime);
		}
	}
	
	void SDLWindow::Shutdown()
	{
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();

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
