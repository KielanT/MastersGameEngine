#pragma once

#include <string>
#include <Windows.h>


namespace Engine
{
	typedef unsigned int eint32;
	typedef unsigned long long eint64;

	enum class ERendererType
	{
		None = 0,
		DirectX11,
	};

	struct WindowProperties
	{
		std::string Title;
		eint32 Width;
		eint32 Height;
		ERendererType RendererType;

		HWND Hwnd = NULL;

		WindowProperties(const std::string& title = "Game Engine",
			eint32 width = 1600, eint32 height = 900,
			ERendererType rendererType = ERendererType::DirectX11) : Title(title), Width(width), Height(height), RendererType(rendererType)
		{
		}
		
	};
}