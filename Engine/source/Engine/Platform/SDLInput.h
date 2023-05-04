#pragma once

#include <SDL.h>

namespace Engine
{
	

	class SDLInput
	{
		// Sets up all the inputs
	public:
		static void InitInput();
		
		static bool KeyHit(SDL_KeyCode key);
		static bool KeyHeld(SDL_KeyCode key);
		
		static void KeyEvent(const SDL_KeyboardEvent& event);

		static uint32_t MouseClick(uint32_t button, glm::ivec2* point = nullptr);
		static uint32_t MouseHeld(uint32_t button, glm::ivec2* point = nullptr);

		static void MouseButtonEvent(const SDL_MouseButtonEvent& event);
		static void MouseMotionEvent(const SDL_MouseMotionEvent& event);

		static glm::ivec2 GetMousePos() { return m_MousePos; }
		static glm::ivec2 GetLastMousePos() { return m_LastMousePos; }
		static glm::ivec2 GetMouseMovement() { return m_MouseMovement; }
	
		enum EKeyPressedState
		{
			NotPressed,
			Pressed,
			Held,
		};

		struct MouseButtonStates
		{
			EKeyPressedState PressedState;
			glm::ivec2 Point;
			eint32 Clicks;
		};
		

	//private:
		static EKeyPressedState m_Keys[SDL_NUM_SCANCODES];
		static MouseButtonStates m_MouseButton[6];

		static glm::ivec2 m_MousePos;
		static glm::ivec2 m_MouseMovement;

		static glm::ivec2 m_LastMousePos;
	};
}

