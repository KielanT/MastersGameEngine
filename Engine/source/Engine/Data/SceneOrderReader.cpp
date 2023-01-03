#include "epch.h"
#include "SceneOrderReader.h"
#include <yaml-cpp/yaml.h>

namespace Engine
{

	void SceneOrderReader::Read(std::string& path, SceneOrder& scene)
	{

		YAML::Node data = YAML::LoadFile(path + "\\order.txt");

		if (!data["AssetPath"])
			return;

		scene.assetFilePath = data["AssetPath"].as<std::string>();

		auto node = data["Scenes"];
		if (node)
		{
			for (auto it : node)
			{
				scene.sceneOrderVar.push_back({ it["Title"].as<std::string>() , it["Index"].as<eint32>() });
			}
		}
	}
}