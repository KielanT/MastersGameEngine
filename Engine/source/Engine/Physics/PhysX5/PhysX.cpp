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

	void PhysX::CreatePhysicsActor(Entity& entity)
	{
		if (entity.HasComponent<TransformComponent>())
		{
			auto& trans = entity.GetComponent<TransformComponent>();
			if (entity.HasComponent<RigidDynamicComponent>())
			{
				auto& comp = entity.GetComponent<RigidDynamicComponent>();
					

					auto physxTrans = physx::PxTransform({ trans.Position.x ,trans.Position.y, trans.Position.z });

					comp.actor = m_Physics->createRigidDynamic(physxTrans);
					physx::PxShape* boxShape = physx::PxRigidActorExt::createExclusiveShape(*comp.actor, physx::PxBoxGeometry(9.0f, 9.0f, 9.0f), *m_DefaultMaterial);
					//physx::PxShape* boxShape = physx::PxRigidActorExt::createExclusiveShape(*comp.actor, physx::PxBoxGeometry(0.5f, 0.5f, 0.5f), *m_DefaultMaterial);
					m_Scene->addActor(*comp.actor);
			}

			if (entity.HasComponent<RigidStaticComponent>())
			{
				auto& comp = entity.GetComponent<RigidStaticComponent>();

				auto physxTrans = physx::PxTransform({ trans.Position.x ,trans.Position.y, trans.Position.z });

				comp.actor = m_Physics->createRigidStatic(physxTrans);
				physx::PxShape* boxShape = physx::PxRigidActorExt::createExclusiveShape(*comp.actor, physx::PxBoxGeometry(1.0f, 1.0f, 1.0f), *m_DefaultMaterial);
				m_Scene->addActor(*comp.actor);

			}
		}
	}

	void PhysX::UpdatePhysicsActor(Entity& entity)
	{
		if (entity.HasComponent<TransformComponent>() && entity.HasComponent<RigidDynamicComponent>())
		{
			entity.GetComponent<TransformComponent>().Position.x = entity.GetComponent<RigidDynamicComponent>().actor->getGlobalPose().p.x;
			entity.GetComponent<TransformComponent>().Position.y = entity.GetComponent<RigidDynamicComponent>().actor->getGlobalPose().p.y;
			entity.GetComponent<TransformComponent>().Position.z = entity.GetComponent<RigidDynamicComponent>().actor->getGlobalPose().p.z;
		}
	}
}