#pragma once

#include "Scene.h"
#include "Entity.h"

#include <yaml-cpp/yaml.h>

namespace Engine
{
	class SceneWriter
	{
	public: 
		void Write(std::string& path, std::shared_ptr<Scene> scene);

	private:
		void SaveSceneSettings(YAML::Emitter& out, std::shared_ptr<Scene> scene);
		void SaveEntity(YAML::Emitter& out, Entity entity);
	};

}

