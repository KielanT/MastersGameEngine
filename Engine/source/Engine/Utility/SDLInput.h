#pragma once

#include <SDL.h>

namespace Engine
{
	

	class SDLInput
	{
	public:
		static void InitInput();
		
		static bool KeyHit(SDL_KeyCode key);
		static bool KeyHeld(SDL_KeyCode key);
		
		static void KeyEvent(const SDL_KeyboardEvent& event);

		enum EKeyPressedState
		{
			NotPressed,
			Pressed,
			Held,
		};

	//private:
		static EKeyPressedState m_Keys[SDL_NUM_SCANCODES];
		
	};
}

