#include "epch.h"
#include "Physics.h"

#include "PhysicsAPI.h"

namespace Engine
{

	static std::shared_ptr<PhysicsAPI> m_PhysicsAPI = nullptr;

	bool Physics::Init()
	{
		LOG_INFO("Physics Initialising");
		m_PhysicsAPI = Create(EPhysicsAPI::PhysX5);
		
		if (!m_PhysicsAPI)
		{
			return false;
		}
		
		
		m_PhysicsAPI->Init();

		return true;
	}

	void Physics::Shutdown()
	{
		//if(m_PhysicsAPI)
		//	m_PhysicsAPI->Shutdown();
	}

	void Physics::Update(float frameTime)
	{
		if (m_PhysicsAPI)
			m_PhysicsAPI->Update(frameTime);
	}

	void Physics::TempTestFunction(Entity& entity, bool test)
	{
		if (m_PhysicsAPI)
			m_PhysicsAPI->TempTestFunction(entity, test);
	}

}