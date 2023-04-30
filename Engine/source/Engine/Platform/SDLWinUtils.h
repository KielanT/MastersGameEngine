#pragma once

#include <SDL.h>

namespace Engine
{

	class FileDialog
	{
	public:
		static std::string OpenFile(HWND hwnd, const char* filter, std::string path = "", std::string title = "Open");
		static std::string SaveFile(HWND hwnd, const char* filter, std::string path = "", std::string title = "Save");
	};
}
