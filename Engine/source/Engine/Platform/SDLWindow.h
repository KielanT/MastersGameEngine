#pragma once

#include "Engine/Interfaces/IWindow.h"
#include "SDL.h"
#include "Engine/Utility/ITimer.h"
#include "SDLInput.h"

namespace Engine
{
	class SDLWindow : public IWindow
	{
	public:
		SDLWindow(WindowProperties& props);
		virtual ~SDLWindow();

		SDLWindow(const SDLWindow&) = delete;
		SDLWindow(SDLWindow&&) = delete;
		SDLWindow& operator=(const SDLWindow&) = delete;
		SDLWindow& operator=(SDLWindow&&) = delete;

	public:
		virtual void Update() override;
		virtual void Shutdown() override;

		virtual WindowProperties GetWindowProperties() const override { return m_Props; };

	private:
		BOOL Init(WindowProperties& props);

	private:
		WindowProperties m_Props;

		SDL_Window* m_Window;

		ITimer m_Timer;

	};
}

