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
		m_Transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
		m_PVD->connect(*m_Transport, physx::PxPvdInstrumentationFlag::eALL);
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

		m_Scene = CreateScene();

		m_DefaultMaterial = m_Physics->createMaterial(0.5f, 0.5f, 0.6f);

		return true;
	}
	void PhysX::Shutdown()
	{
		//PxCloseExtensions();
		//
		////Causes Error When shutting down */
		//if (m_Physics)
		//	m_Physics->release();
		//
		//if (m_PVD)
		//	m_PVD->release();
		//
		// //Must be last
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
					
				physx::PxTransform transform = SetPhysicsTransform(entity.GetComponent<TransformComponent>());

				comp.actor = m_Physics->createRigidDynamic(transform);
				SetPhysicsSettings(entity.GetComponent<RigidDynamicComponent>());
				m_Scene->addActor(*comp.actor);

			}
		}
	}

	void PhysX::CreateCollision(Entity& entity)
	{
		if (entity.HasComponent<CollisionComponents>())
		{
			auto& collisionComp = entity.GetComponent<CollisionComponents>();
			// if has a static component or rigid attach the shape to it
			if (entity.HasComponent<RigidDynamicComponent>())
			{
				auto& comp = entity.GetComponent<RigidDynamicComponent>();
				if (collisionComp.CollisionType == ECollisionTypes::Box)
				{
					collisionComp.CollisionShape = physx::PxRigidActorExt::createExclusiveShape(*comp.actor, physx::PxBoxGeometry(collisionComp.BoxBounds.x, collisionComp.BoxBounds.y, collisionComp.BoxBounds.z), *m_DefaultMaterial);
				}
				if (collisionComp.CollisionType == ECollisionTypes::Sphere)
				{
					collisionComp.CollisionShape = physx::PxRigidActorExt::createExclusiveShape(*comp.actor, physx::PxSphereGeometry(collisionComp.SphereRadius), *m_DefaultMaterial);
				}
			}
			else // if does not have a component create a new actor
			{
				if (entity.HasComponent<TransformComponent>())
				{
					auto& trans = entity.GetComponent<TransformComponent>();
					
					auto physxTrans = physx::PxTransform({ trans.Position.x ,trans.Position.y, trans.Position.z });

					collisionComp.actor = m_Physics->createRigidStatic(physxTrans);
					m_Scene->addActor(*collisionComp.actor);

					if (collisionComp.CollisionType == ECollisionTypes::Box)
					{
						collisionComp.CollisionShape = physx::PxRigidActorExt::createExclusiveShape(*collisionComp.actor, physx::PxBoxGeometry(collisionComp.BoxBounds.x, collisionComp.BoxBounds.y, collisionComp.BoxBounds.z), *m_DefaultMaterial);
					}
					if (collisionComp.CollisionType == ECollisionTypes::Sphere)
					{
						collisionComp.CollisionShape = physx::PxRigidActorExt::createExclusiveShape(*collisionComp.actor, physx::PxSphereGeometry(collisionComp.SphereRadius), *m_DefaultMaterial);
					}
				}

				
			}
		}
		
	}

	void PhysX::UpdatePhysicsActor(Entity& entity)
	{
		if (entity.HasComponent<TransformComponent>() && entity.HasComponent<RigidDynamicComponent>())
		{
			SetRenderedTransform(entity.GetComponent<TransformComponent>(), entity.GetComponent<RigidDynamicComponent>().actor->getGlobalPose());
			SetPhysicsSettings(entity.GetComponent<RigidDynamicComponent>());
			
		}
		if (entity.HasComponent<TransformComponent>() && entity.HasComponent<CollisionComponents>())
		{
			if (entity.GetComponent<CollisionComponents>().actor != nullptr)
			{
				SetRenderedTransform(entity.GetComponent<TransformComponent>(), entity.GetComponent<CollisionComponents>().actor->getGlobalPose());
			}
		}

	}

	void PhysX::EditorUpdateActors(Entity& entity)
	{
		if (entity.HasComponent<TransformComponent>() && entity.HasComponent<RigidDynamicComponent>())
		{
			physx::PxTransform transform = SetPhysicsTransform(entity.GetComponent<TransformComponent>());
			entity.GetComponent<RigidDynamicComponent>().actor->setGlobalPose(transform);
		}
		if (entity.HasComponent<TransformComponent>() && entity.HasComponent<CollisionComponents>())
		{
			if (entity.GetComponent<CollisionComponents>().actor != nullptr)
			{
				physx::PxTransform transform = SetPhysicsTransform(entity.GetComponent<TransformComponent>());
				entity.GetComponent<CollisionComponents>().actor->setGlobalPose(transform);
			}
		}
	}

	void PhysX::ResetSimulation()
	{
		if (m_Scene != nullptr)
		{
			m_Scene->release();
			m_Scene = nullptr;
		}
		m_PVD->disconnect();
		m_Scene = CreateScene();
		
		//m_Scene->flushSimulation();
		//m_Scene->flushUpdates();
		// https://forums.developer.nvidia.com/t/how-to-reset-simulation-in-physx-3-3/47494
	}

	physx::PxScene* PhysX::CreateScene()
	{
		physx::PxScene* scene = nullptr;
		physx::PxSceneDesc sceneDesc(m_Physics->getTolerancesScale());
		sceneDesc.gravity = physx::PxVec3(0.0f, -9.81, 0.0f);
		m_CpuDispatcher = physx::PxDefaultCpuDispatcherCreate(2);
		sceneDesc.cpuDispatcher = m_CpuDispatcher;
		sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;

		scene = m_Physics->createScene(sceneDesc);
		scene->setVisualizationParameter(physx::PxVisualizationParameter::eCOLLISION_SHAPES, 1.0f);
		scene->setVisualizationParameter(physx::PxVisualizationParameter::eSCALE, 1.0f);

		if (!m_PVD->isConnected())
		{
			m_PVD->connect(*m_Transport, physx::PxPvdInstrumentationFlag::eALL);
		}
		return scene;
	}

	void PhysX::SetRenderedTransform(TransformComponent& transform, physx::PxTransform pTransform)
	{
		transform.Position.x = pTransform.p.x;
		transform.Position.y = pTransform.p.y;
		transform.Position.z = pTransform.p.z;

		glm::quat rot;
		rot.x = pTransform.q.x;
		rot.y = pTransform.q.y;
		rot.z = pTransform.q.z;
		rot.w = pTransform.q.w;

		transform.Rotation = glm::eulerAngles(rot * 3.14159f / 180.f);
	}

	physx::PxTransform PhysX::SetPhysicsTransform(TransformComponent& transform)
	{
		auto position = transform.Position;
		position.z = -position.z;
		auto rotation = transform.Rotation;
		glm::quat gQuat = glm::quat(rotation);
		physx::PxQuat pQuat;
		pQuat.x = gQuat.x;
		pQuat.y = gQuat.y;
		pQuat.z = gQuat.z;
		pQuat.w = gQuat.w;

		physx::PxTransform pTransform;
		pTransform.p = { position.x, position.y, position.z };
		pTransform.q = pQuat;

		return pTransform;
	}

	void PhysX::SetPhysicsSettings(RigidDynamicComponent& comp)
	{
		comp.actor->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, !comp.Gravity);
		comp.actor->setMass(comp.Mass);
		comp.actor->setMassSpaceInertiaTensor({ comp.MassSpaceInertiaTensor.x, comp.MassSpaceInertiaTensor.y,comp.MassSpaceInertiaTensor.z });
		//comp.actor->setLinearVelocity({ (physx::PxReal)comp.LinearVelocity.x, (physx::PxReal)comp.LinearVelocity.y, (physx::PxReal)comp.LinearVelocity.z });
		//comp.actor->setAngularDamping(comp.AngularDamping);
	}
	
}