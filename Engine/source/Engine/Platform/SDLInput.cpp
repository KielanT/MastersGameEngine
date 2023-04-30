#include "epch.h"
#include "SDLInput.h"

namespace Engine
{
	SDLInput::EKeyPressedState SDLInput::m_Keys[SDL_NUM_SCANCODES];
	SDLInput::MouseButtonStates SDLInput::m_MouseButton[6];
	glm::ivec2 SDLInput::m_MousePos = { 0, 0 };
	glm::ivec2 SDLInput::m_MouseMovement = { 0, 0 };
	glm::ivec2 SDLInput::m_LastMousePos = { 0, 0 };
	
	void SDLInput::InitInput()
	{
		for (uint32_t i = 0; i < SDL_NUM_SCANCODES; i++)
		{
			m_Keys[i] = NotPressed;
		}

		for (uint32_t i = 0; i < 6; i++)
		{
			m_MouseButton[i].PressedState = NotPressed;
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
	
	uint32_t SDLInput::MouseClick(uint32_t button, glm::ivec2* point)
	{
		if (m_MouseButton[button].PressedState != Pressed)  return false;
		if (point)    *point = m_MouseButton[button].Point;
		m_MouseButton[button].PressedState = Pressed;
		return m_MouseButton[button].Clicks;
	}
	
	uint32_t SDLInput::MouseHeld(uint32_t button, glm::ivec2* point) 
	{
		if (m_MouseButton[button].PressedState == NotPressed)  return false;
		if (point) 
		{
			*point = m_MouseButton[button].Point;
			
		}
		m_MouseButton[button].PressedState = Held;
		

		return m_MouseButton[button].Clicks;

	}

	void SDLInput::MouseButtonEvent(const SDL_MouseButtonEvent& event) 
	{
		if (event.type == SDL_MOUSEBUTTONDOWN && event.which != SDL_TOUCH_MOUSEID) // Ignore touch events
		{
			m_MouseButton[event.button].PressedState = Pressed;
			m_MouseButton[event.button].Point = glm::ivec2(event.x, event.y);
			m_MouseButton[event.button].Clicks = event.clicks;
			m_LastMousePos = glm::ivec2(event.x, event.y);
			
		}
		else if (event.type == SDL_MOUSEBUTTONUP)
		{
			m_MouseButton[event.button].PressedState = NotPressed;
		}
	}

	void SDLInput::MouseMotionEvent(const SDL_MouseMotionEvent& event)
	{
		if (event.type == SDL_MOUSEMOTION && event.which != SDL_TOUCH_MOUSEID) // Ignore touch events
		{
			m_MousePos = glm::ivec2(event.x, event.y);
			m_MouseMovement += glm::ivec2(event.xrel, event.yrel);
			
	
		}
	}
}