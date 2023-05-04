#pragma once

#include "PhysXCustomCallbacks.h"
#include "PhysXCollisionCallbacks.h"
#include <PxPhysicsAPI.h>
#include "Engine/Scene/Entity.h"

namespace Engine
{
	/**********************************************************
	* 
	*	PhysX singleton for using the physics object anywhere
	* 
	**********************************************************/

	class PhysX  
	{
	public:
		~PhysX();

		// Main Physics functions
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
		// Create the physics scene
		physx::PxScene* CreateScene();

		// Sets the rendered transfrom to the physics transform
		void SetRenderedTransform(TransformComponent& transform, physx::PxTransform pTransform);

		// Set physics transform
		physx::PxTransform SetPhysicsTransform(TransformComponent& transform);

		// Set physics settings to the component
		void SetPhysicsSettings(RigidDynamicComponent& comp);

	private:
		// Instance for the singleton
		static std::shared_ptr<PhysX> m_Instance;

		// PhysX variables required for the physics settings
		physx::PxFoundation* m_Foundation;
		physx::PxPvd* m_PVD;
		physx::PxPhysics* m_Physics;
		physx::PxScene* m_Scene;
		physx::PxDefaultCpuDispatcher* m_CpuDispatcher = NULL;
		physx::PxMaterial* m_DefaultMaterial;
		physx::PxPvdTransport* m_Transport;


		
	};

	
}
