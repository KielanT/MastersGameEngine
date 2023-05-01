#include "epch.h"
#include "PhysXCollisionCallbacks.h"
#include "Engine/Physics/PhysX5/PhysX.h"

static Engine::Entity GetEntity(physx::PxActor* actor)
{
	
	if (auto instance = Engine::PhysX::GetInstance()->EntityMap.find(actor); instance != Engine::PhysX::GetInstance()->EntityMap.end())
	{
		return instance->second;
	}
	return Engine::Entity();
}

void PhysXCollisionCallbacks::onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count)
{
	LOG_DEBUG("onConstraintBreak");
}

void PhysXCollisionCallbacks::onWake(physx::PxActor** actors, physx::PxU32 count)
{
	LOG_DEBUG("onWake");
}

void PhysXCollisionCallbacks::onSleep(physx::PxActor** actors, physx::PxU32 count)
{
	LOG_DEBUG("onSleep");
}

void PhysXCollisionCallbacks::onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count)
{
	LOG_DEBUG("onTrigger");
}

void PhysXCollisionCallbacks::onAdvance(const physx::PxRigidBody* const*, const physx::PxTransform*, const physx::PxU32)
{
	LOG_DEBUG("onAdvance");
}

void PhysXCollisionCallbacks::onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 count)
{
	LOG_DEBUG("OnContact");

	for (physx::PxU32 i = 0; i < count; i++)
	{
		const physx::PxContactPair& pair = pairs[i];
		Engine::Entity entity1 = GetEntity(pairHeader.actors[0]);
		Engine::Entity entity2 = GetEntity(pairHeader.actors[1]);
		
		entity1.OnContact(entity2); // Notify entity1 of contact with entity2 at first contact point
		entity2.OnContact(entity1); // Notify entity2 of contact with entity1 at first contact point

	}
}
