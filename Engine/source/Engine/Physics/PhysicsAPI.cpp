#include "epch.h"
#include "PhysicsAPI.h"
#include "PhysX5/PhysX.h"

namespace Engine
{
    std::shared_ptr<PhysicsAPI> Create(const EPhysicsAPI API)
    {
		switch (API)
		{
		case EPhysicsAPI::None:
			return nullptr;
			case EPhysicsAPI::PhysX5:
			return std::make_shared<PhysX>();
		}

		LOG_ERROR("Physics API Creation Failed");
		return nullptr;
    }

}