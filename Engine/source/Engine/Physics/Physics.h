#pragma once
#include "Engine/Scene/Entity.h"

namespace Engine
{
	class Physics
	{
	public:

		static bool Init();
		static void Shutdown();
		static void Update(float frameTime);
		static void TempTestFunction(Entity& entity, bool test);
	};
}
