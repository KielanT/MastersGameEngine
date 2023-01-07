#include "epch.h"
#include "PhysX.h"

namespace Engine
{
	static PhysXCustomErrorCallback gCustomErrorCallback;
	static physx::PxDefaultAllocator gDefaultAllocatorCallback;

	PhysX::~PhysX()
	{
		Shutdown();
	}

	bool PhysX::Init()
	{
		LOG_INFO("PhysX 5 Initilised");

		m_Foundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gCustomErrorCallback);
		if (!m_Foundation)
		{
			LOG_ERROR("PxCreateFoundation failed!");
			return false;
		}

		m_PVD = physx::PxCreatePvd(*m_Foundation);
		physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
		m_PVD->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);
		if (!m_PVD)
			LOG_ERROR("Failed to Create PVD (Does not Break Engine)");

		bool recordMemoryAllocations = false;
		m_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation, physx::PxTolerancesScale(), recordMemoryAllocations, m_PVD);
		if (!m_Physics)
		{
			LOG_ERROR("PxCreatePhysics failed!");
			return false;
		}

		if(!PxInitExtensions(*m_Physics, m_PVD))
		{
			LOG_ERROR("PxInitExtensions failed! "); 
			return false;
		}

		physx::PxSceneDesc sceneDesc(m_Physics->getTolerancesScale());
		sceneDesc.gravity = physx::PxVec3(0.0f, -9.81, 0.0f);
		m_CpuDispatcher = physx::PxDefaultCpuDispatcherCreate(2);
		sceneDesc.cpuDispatcher = m_CpuDispatcher;
		sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;

		m_Scene = m_Physics->createScene(sceneDesc);
		m_Scene->setVisualizationParameter(physx::PxVisualizationParameter::eCOLLISION_SHAPES, 1.0f);
		m_Scene->setVisualizationParameter(physx::PxVisualizationParameter::eSCALE, 1.0f);

		m_DefaultMaterial = m_Physics->createMaterial(0.5f, 0.5f, 0.6f);

		// TEMP Cube
		actor = m_Physics->createRigidDynamic(physx::PxTransform({ 0.0f, 0.0f, 0.0f }));
		physx::PxShape* boxShape = physx::PxRigidActorExt::createExclusiveShape(*actor, physx::PxBoxGeometry(1.0f, 1.0f, 1.0f), *m_DefaultMaterial);
		m_Scene->addActor(*actor);
		actor->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, true);

		return true;
	}
	void PhysX::Shutdown()
	{
		//PxCloseExtensions();

		/*Causes Error When shutting down */
		//if (m_Physics)
		//	m_Physics->release();

		//if (m_PVD)
		//	m_PVD->release();

		// Must be last
		//if (m_Foundation)
		//	m_Foundation->release();
		
	}
	void PhysX::Update(float frameTime)
	{
		m_Scene->simulate(frameTime);
		m_Scene->fetchResults(true);
	}

	void PhysX::TempTestFunction(Entity& entity, bool test)
	{
		static int i = 0;
		if (test)
		{
			if (i == 0)
			{
				actor->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, false);
				actor->wakeUp();
				i++;
			}
			entity.GetComponent<TransformComponent>().Position.x = actor->getGlobalPose().p.x;
			entity.GetComponent<TransformComponent>().Position.y = actor->getGlobalPose().p.y;
			entity.GetComponent<TransformComponent>().Position.z = actor->getGlobalPose().p.z;
		}


	}
}