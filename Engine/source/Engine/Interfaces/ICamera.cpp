#include "epch.h"
#include "ICamera.h"

namespace Engine 
{
	std::shared_ptr<ICamera> CreateCamera(const ECameraType type) 
	{
		if (type == ECameraType::Game) 
		{
			return
		}
		else if (type == ECameraType::Editor)
		{
			return 
		}
		else 
		{
			return nullptr;
		}
	}
}