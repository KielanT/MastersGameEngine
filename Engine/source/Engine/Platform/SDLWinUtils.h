#pragma once

#include <SDL.h>

namespace Engine
{

	class FileDialog
	{
	public:
		static std::string OpenFile(HWND hwnd, const char* filter);
		static std::string SaveFile(HWND hwnd, const char* filter);
	};
}
