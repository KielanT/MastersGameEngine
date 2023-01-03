#include "Engine/Scene/Scene.h"
#include "Engine/Scene/Entity.h"
#include <yaml-cpp/yaml.h>

namespace Engine
{
	class SceneReader
	{
	public:
		void Read(std::string& path, std::shared_ptr<Scene> scene);
	};

	class SceneOrderReader
	{
	public:
		void Read(std::string& path, SceneOrder& scene);
	};
}