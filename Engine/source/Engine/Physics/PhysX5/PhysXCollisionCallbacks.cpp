#include "epch.h"
#include "PhysXCollisionCallbacks.h"

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
	
}
