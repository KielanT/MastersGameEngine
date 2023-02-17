#include "epch.h"
#include "Serializer.h"
#include "Writers.h"
#include "Readers.h"


namespace Engine
{
	void SceneSerializer::SerializeScene(std::string& path, std::shared_ptr<Scene> scene)
	{
		SceneWriter writer;
		writer.Write(path, scene);
	}

	void SceneSerializer::DeserializeScene(std::string& path, std::shared_ptr<Scene> scene)
	{
		SceneReader reader;
		reader.Read(path, scene);
	}



	void SettingsSerilizer::SerializeEditorSettings(std::string& path, EditorSettings& settings)
	{
		EditorSettingsWriter writer;
		writer.Write(path, settings);
	}

	void SettingsSerilizer::DeserializeEditorSettings(std::string path, EditorSettings& settings)
	{
		EditorSettingsReader reader;
		if (!reader.Read(path, settings))
		{
			LOG_WARN("Unable to read editor settings");
		}

	}
}