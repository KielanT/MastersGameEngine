#include "epch.h"
#include "Physics.h"

#include "PhysicsAPI.h"

namespace Engine
{

	static std::shared_ptr<PhysicsAPI> m_PhysicsAPI = nullptr;

	bool Physics::Init()
	{
		if (m_PhysicsAPI)
		{
			return true;
		}

		m_PhysicsAPI = Create(EPhysicsAPI::PhysX5);
		
		LOG_INFO("Physics Initialising");
		if (!m_PhysicsAPI)
		{
			LOG_ERROR("Physics Failed");
			return false;
		}
		
		
		if (!m_PhysicsAPI->Init())
		{
			LOG_ERROR("Physics Failed");
			return false;
		}

		return true;
	}

	void Physics::Shutdown()
	{
		if(m_PhysicsAPI)
			m_PhysicsAPI->Shutdown();
	}

	void Physics::Update(float frameTime)
	{
		if (m_PhysicsAPI)
			m_PhysicsAPI->Update(frameTime);
	}

	void Physics::CreatePhysicsActor(Entity& entity)
	{
		if (m_PhysicsAPI)
			m_PhysicsAPI->CreatePhysicsActor(entity);
	}

	void Physics::CreateCollision(Entity& entity)
	{
		if (m_PhysicsAPI)
			m_PhysicsAPI->CreateCollision(entity);
	}

	void Physics::UpdatePhysicsActor(Entity& entity)
	{
		if (m_PhysicsAPI)
			m_PhysicsAPI->UpdatePhysicsActor(entity);
	}

	void Physics::ResetSimulation()
	{
		if (m_PhysicsAPI)
			m_PhysicsAPI->ResetSimulation();
	}


}