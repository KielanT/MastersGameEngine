#pragma once

#include "Engine/Utility/ProjectDefines.h"


namespace Engine
{
	class IWindow
	{
	public:
		virtual ~IWindow() = default;

		virtual void Update() = 0;
		virtual void Shutdown() = 0;
		
		virtual WindowProperties GetWindowProperties() const = 0;
		
		static IWindow* Create(WindowProperties& props);
	};

}