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
		static void CreatePhysicsActor(Entity& entity);
		static void CreateCollision(Entity& entity);
		static void UpdatePhysicsActor(Entity& entity);
		static void EditorUpdateActors(Entity& entity);
		static void ResetSimulation();
	};
}
