#pragma once
#include "Engine/Scene/Entity.h"

enum class EPhysicsAPI
{
	None = 0,
	PhysX5
	
};

namespace Engine
{
	class PhysicsAPI
	{
	public:
		virtual bool Init() = 0;
		virtual void Shutdown() = 0;
		virtual void Update(float frameTime) = 0;

		virtual void TempTestFunction(Entity& entity, bool test) = 0;
	};

	std::shared_ptr<PhysicsAPI> Create(const EPhysicsAPI API); // Create Renderer
}