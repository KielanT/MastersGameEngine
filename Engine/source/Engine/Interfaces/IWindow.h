#pragma once

#include "Engine/Utility/ProjectDefines.h"
#include "Engine/Interfaces/ISceneManager.h"

namespace Engine
{
	class IWindow
	{
	public:
		virtual ~IWindow() = default;

		//virtual void Update(std::shared_ptr<ISceneManager> m_SceneManager) = 0;
		virtual void Update() = 0;
		virtual void Shutdown() = 0;
		
		virtual WindowProperties GetWindowProperties() const = 0;
		
		static std::shared_ptr<IWindow> Create(WindowProperties& props);
	};

}