#pragma once

#include "Engine/Physics/PhysicsAPI.h"
#include "PhysXCustomCallbacks.h"
#include <PxPhysicsAPI.h>

namespace Engine
{


	class PhysX : public PhysicsAPI
	{
	public:
		~PhysX();

		virtual bool Init() override;
		virtual void Shutdown() override;
		virtual void Update(float frameTime) override;
		virtual void CreatePhysicsActor(Entity& entity) override;
		virtual void UpdatePhysicsActor(Entity& entity) override;
		virtual void ResetSimulation() override;

	private:
		physx::PxFoundation* m_Foundation;
		physx::PxPvd* m_PVD;
		physx::PxPhysics* m_Physics;
		physx::PxScene* m_Scene;

		
		physx::PxDefaultCpuDispatcher* m_CpuDispatcher = NULL;
		
		physx::PxMaterial* m_DefaultMaterial;
	};

	
}
