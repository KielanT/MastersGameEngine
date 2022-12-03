#pragma once

#include <SDL.h>

namespace Engine
{

	std::string OpenFile(HWND hwnd, const char* filter)
	{
		OPENFILENAMEA ofn;
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		char Buffer[300];
		std::fill(Buffer, Buffer + 300, '\0');
		ofn.lStructSize = sizeof(OPENFILENAMEA);
		ofn.hwndOwner = hwnd;
		ofn.lpstrFile = Buffer;
		ofn.nMaxFile = sizeof(Buffer);
		ofn.Flags = OFN_EXPLORER;
		ofn.lpstrFilter = filter;
		ofn.lpstrCustomFilter = NULL;
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.lpstrInitialDir = NULL;
		ofn.lpstrTitle = "Open";

		if (GetOpenFileNameA(&ofn) == TRUE)
			return ofn.lpstrFile;

		return std::string();
	}

}
