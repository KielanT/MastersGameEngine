#pragma once

#include "Engine/Interfaces/ILayer.h"

namespace Engine
{
	class Layer
	{
	public:
		static bool Init(std::shared_ptr<ILayer> layer);
		static void Render();
		static void Update(float frameTime);
	};
}

