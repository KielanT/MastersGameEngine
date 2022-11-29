#pragma once

#include "IRenderer.h"

namespace Engine
{ 
	// State interface used for creating states depending on the renderer
	class  IState
	{
	public:
		virtual ~IState() {}

		// Initialize the states 
		virtual bool InitStates(std::shared_ptr<IRenderer> renderer) = 0;

		// Release the states
		virtual void ReleaseStates() = 0;
	};

	// Returns the states depending on the renderer
	std::shared_ptr<IState> SetStates(ERendererType type);
}