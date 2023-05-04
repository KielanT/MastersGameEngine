#include "epch.h"
#include "SDLWinUtils.h"

namespace Engine
{
	std::string FileDialog::OpenFile(HWND hwnd, const char* filter, std::string path, std::string title)
	{
		// opens the dialog with settings
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

		if (!path.empty())
			ofn.lpstrInitialDir = path.c_str();
		else
			ofn.lpstrInitialDir = NULL;

		ofn.lpstrTitle = title.c_str();

		if (GetOpenFileNameA(&ofn))
			return ofn.lpstrFile;

		return std::string();
	}

	std::string FileDialog::SaveFile(HWND hwnd, const char* filter, std::string path, std::string title)
	{
		// Opens the save diaglog
		OPENFILENAMEA ofn;
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		char Buffer[300];
		std::fill(Buffer, Buffer + 300, '\0');
		ofn.lStructSize = sizeof(OPENFILENAMEA);
		ofn.hwndOwner = hwnd;
		ofn.lpstrFile = Buffer;
		ofn.nMaxFile = 300;
		ofn.Flags = OFN_EXPLORER;
		ofn.lpstrFilter = filter;
		ofn.lpstrCustomFilter = NULL;
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;

		if(!path.empty())
			ofn.lpstrInitialDir = path.c_str();
		else
			ofn.lpstrInitialDir = NULL;
		
		ofn.lpstrTitle = title.c_str();
		ofn.lpstrDefExt = strchr(filter, '\0') + 1;

		if (GetSaveFileNameA(&ofn))
		{
			return ofn.lpstrFile;
		}

		return std::string();
	}
}