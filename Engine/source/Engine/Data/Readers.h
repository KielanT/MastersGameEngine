#include "Engine/Scene/Scene.h"
#include "Engine/Scene/Entity.h"
#include "Engine/Data/Settings.h"

#include <yaml-cpp/yaml.h>

namespace Engine
{
	class SceneReader
	{
	public:
		// Read the scene settings
		void Read(std::string& path, std::shared_ptr<Scene> scene);
	};

	class EditorSettingsReader
	{
	public:
		// Read the editor settings
		bool Read(std::string& path, EditorSettings& settings);
	};

}