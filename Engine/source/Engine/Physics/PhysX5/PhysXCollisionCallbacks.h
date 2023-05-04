#pragma once

#include <PxSimulationEventCallback.h>
#include "Engine/Scene/Entity.h"

class PhysXCollisionCallbacks : public physx::PxSimulationEventCallback
{
	// Simulation call backs
	void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count);

	void onWake(physx::PxActor** actors, physx::PxU32 count);

	void onSleep(physx::PxActor** actors, physx::PxU32 count);

	// Collision call backs
	void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count);

	void onAdvance(const physx::PxRigidBody* const*, const physx::PxTransform*, const physx::PxU32);

	void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 count);
};

static  physx::PxFilterFlags contactReportFilterShader(physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0,
	physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1,
	physx::PxPairFlags& pairFlags, const void* constantBlock, physx::PxU32 constantBlockSize)
{
	PX_UNUSED(attributes0);
	PX_UNUSED(attributes1);
	PX_UNUSED(filterData0);
	PX_UNUSED(filterData1);
	PX_UNUSED(constantBlockSize);
	PX_UNUSED(constantBlock);

	// all initial and persisting reports for everything, with per-point data
	pairFlags = physx::PxPairFlag::eSOLVE_CONTACT | physx::PxPairFlag::eDETECT_DISCRETE_CONTACT
		|  physx::PxPairFlag::eNOTIFY_TOUCH_FOUND
		|  physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS
		|  physx::PxPairFlag::eNOTIFY_CONTACT_POINTS;
	return  physx::PxFilterFlag::eDEFAULT;
}
