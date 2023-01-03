
#include "Engine/Scene/Scene.h"
#include "Engine/Scene/Entity.h"

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

	class SceneOrderWriter
	{
	public:
		void Write(std::string& path, SceneOrder scene);
	};
}
