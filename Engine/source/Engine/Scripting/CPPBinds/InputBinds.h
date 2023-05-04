#pragma once

#include "Engine/Platform/SDLInput.h"

//*********************************************
// Creates the code that get called from C# 
// For the inputs
//*********************************************

static bool Input_KeyHeld(int keycode)
{
	return Engine::SDLInput::KeyHeld((SDL_KeyCode)keycode);
}

static bool Input_KeyHit(int keycode)
{
	return Engine::SDLInput::KeyHit((SDL_KeyCode)keycode);
}