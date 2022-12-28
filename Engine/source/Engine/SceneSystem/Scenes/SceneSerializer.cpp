#include "epch.h"
#include "SceneSerializer.h"
#include "SceneWriter.h"
#include "SceneReader.h"

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
}