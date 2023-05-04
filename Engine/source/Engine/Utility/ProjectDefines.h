#pragma once

#include <string>
#include <Windows.h>

typedef unsigned int eint32;
typedef unsigned long long eint64;

namespace Engine
{
	
	// Project data types that are required in multiple parts of the engine
	enum class ERendererType
	{
		None = 0,
		DirectX11,
	};

	enum class EShaderType
	{
		PBR = 0,
		Skybox,
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