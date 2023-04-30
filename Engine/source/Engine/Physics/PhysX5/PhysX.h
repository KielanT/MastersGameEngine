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
		virtual void CreateCollision(Entity& entity) override;
		virtual void UpdatePhysicsActor(Entity& entity) override;
		virtual void EditorUpdateActors(Entity& entity) override;
		virtual void ResetSimulation() override;

	private:
		physx::PxScene* CreateScene();

		void SetRenderedTransform(TransformComponent& transform, physx::PxTransform pTransform);
		physx::PxTransform SetPhysicsTransform(TransformComponent& transform);

		void SetPhysicsSettings(RigidDynamicComponent& comp);

	private:
		physx::PxFoundation* m_Foundation;
		physx::PxPvd* m_PVD;
		physx::PxPhysics* m_Physics;
		physx::PxScene* m_Scene;

		
		physx::PxDefaultCpuDispatcher* m_CpuDispatcher = NULL;
		
		physx::PxMaterial* m_DefaultMaterial;

		physx::PxPvdTransport* m_Transport;
	};

	
}
