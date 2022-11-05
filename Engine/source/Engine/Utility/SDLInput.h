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

		//struct SKeyState
		//{
		//	EKeyPressedState PressedState;
		//	uint32_t         WindowID;
		//	SDL_Keymod       ModKeys;
		//};

	//private:
		static EKeyPressedState m_Keys[SDL_NUM_SCANCODES];
		
	};
}

