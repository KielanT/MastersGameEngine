#pragma once

#include "Engine/Scene/Scene.h"

// Static Class to for the serilisation 

namespace Engine
{
	class SceneSerializer
	{
	public:
		static void SerializeScene(std::string& path, std::shared_ptr<Scene> scene);
		static void DeserializeScene(std::string& path, std::shared_ptr<Scene> scene);
	};

	class SceneOrderSerilizer
	{
	public:
		static void SerializeSceneOrder(std::string& path, SceneOrder scene);
		static void DeserializeSceneOrder(std::string& path, SceneOrder& scene);
	};
}

