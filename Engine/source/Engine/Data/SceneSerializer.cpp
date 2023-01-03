#include "epch.h"
#include "SceneSerializer.h"
#include "SceneWriter.h"
#include "SceneReader.h"
#include "SceneOrderWriter.h"
#include "SceneOrderReader.h"

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

	void SceneOrderSerilizer::SerializeSceneOrder(std::string& path, SceneOrder scene)
	{
		SceneOrderWriter writer;
		writer.Write(path, scene);
	}

	
	void SceneOrderSerilizer::DeserializeSceneOrder(std::string& path, SceneOrder& scene)
	{
		SceneOrderReader reader;
		reader.Read(path, scene);
	}
}