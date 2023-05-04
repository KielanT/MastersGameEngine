
#include "Engine/Scene/Scene.h"
#include "Engine/Scene/Entity.h"
#include "Engine/Data/Settings.h"

#include <yaml-cpp/yaml.h>

namespace Engine
{
	class SceneWriter
	{
	public:
		// Write the scene to file
		void Write(std::string& path, std::shared_ptr<Scene> scene);

	private:
		// Helper functions for saving
		void SaveSceneSettings(YAML::Emitter& out, std::shared_ptr<Scene> scene);
		void SaveEntity(YAML::Emitter& out, Entity entity);
	};


	class EditorSettingsWriter
	{
	public:
		// Write the setting to file
		void Write(std::string& path, EditorSettings settings);
	};
}
