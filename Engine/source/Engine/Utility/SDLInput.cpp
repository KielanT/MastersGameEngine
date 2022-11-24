#include "epch.h"
#include "SDLInput.h"

namespace Engine
{
	SDLInput::EKeyPressedState SDLInput::m_Keys[SDL_NUM_SCANCODES];
	
	void SDLInput::InitInput()
	{
		for (uint32_t i = 0; i < SDL_NUM_SCANCODES; i++)
		{
			m_Keys[i] = NotPressed;
		}
	}

	bool SDLInput::KeyHit(SDL_KeyCode key)
	{
		SDL_Scancode scancode = SDL_GetScancodeFromKey(key);
		if (m_Keys[scancode] != Pressed) return false;
		m_Keys[scancode] = Pressed;
		return true;

	}
	
	bool SDLInput::KeyHeld(SDL_KeyCode key)
	{
		SDL_Scancode scancode = SDL_GetScancodeFromKey(key);
		if (m_Keys[scancode] == NotPressed) return false;
		m_Keys[scancode] = Held;
		return true;
	}

	void SDLInput::KeyEvent(const SDL_KeyboardEvent& event)
	{
		if (event.type == SDL_KEYDOWN /*&& event.repeat == 0*/)
		{
			m_Keys[event.keysym.scancode] = Pressed;
		}
		else if (event.type == SDL_KEYUP)
		{
			m_Keys[event.keysym.scancode] = NotPressed;
		}
	}
	
}