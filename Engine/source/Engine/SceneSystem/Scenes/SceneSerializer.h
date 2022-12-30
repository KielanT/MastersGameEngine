#pragma once

#include "Scene.h"

// Static Class to for the serilisation 

namespace Engine
{
	class SceneSerializer
	{
	public:
		static void SerializeScene(std::string& path, std::shared_ptr<Scene> scene);
		static void DeserializeScene(std::string& path, std::shared_ptr<Scene> scene);
	};
}
