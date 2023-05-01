#pragma once

#include "PhysXCustomCallbacks.h"
#include "PhysXCollisionCallbacks.h"
#include <PxPhysicsAPI.h>
#include "Engine/Scene/Entity.h"

namespace Engine
{


	class PhysX  
	{
	public:
		~PhysX();

		bool Init();
		void Shutdown();
		void Update(float frameTime);
		void CreatePhysicsActor(Entity& entity);
		void CreateCollision(Entity& entity);
		void UpdatePhysicsActor(Entity& entity);
		void EditorUpdateActors(Entity& entity);
		void ResetSimulation();

		// Map Used for collisions
		std::unordered_map<physx::PxActor*, Entity> EntityMap;

		static std::shared_ptr<PhysX> GetInstance();

	private:
		physx::PxScene* CreateScene();

		void SetRenderedTransform(TransformComponent& transform, physx::PxTransform pTransform);
		physx::PxTransform SetPhysicsTransform(TransformComponent& transform);

		void SetPhysicsSettings(RigidDynamicComponent& comp);

	private:
		static std::shared_ptr<PhysX> m_Instance;

		physx::PxFoundation* m_Foundation;
		physx::PxPvd* m_PVD;
		physx::PxPhysics* m_Physics;
		physx::PxScene* m_Scene;

		
		physx::PxDefaultCpuDispatcher* m_CpuDispatcher = NULL;
		
		physx::PxMaterial* m_DefaultMaterial;

		physx::PxPvdTransport* m_Transport;


		
	};

	
}
