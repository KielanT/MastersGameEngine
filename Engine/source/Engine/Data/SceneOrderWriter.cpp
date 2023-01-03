#include "epch.h"
#include "SceneOrderWriter.h"

namespace Engine
{
	void SceneOrderWriter::Write(std::string& path, SceneOrder scene)
	{
		YAML::Emitter out;

		out << YAML::BeginMap;
		out << YAML::Key << "AssetPath" << YAML::Value << scene.assetFilePath;

		out << YAML::Key << "Scenes" << YAML::BeginSeq;
		for (auto var : scene.sceneOrderVar)
		{
			out << YAML::BeginMap;
			out << YAML::Key << "Title" << YAML::Value << var.title;
			out << YAML::Key << "Index" << YAML::Value << var.index;
			out << YAML::EndMap; 
		}
		out << YAML::EndSeq;

		out << YAML::EndMap;

		std::ofstream fout(path + "/order.txt");
		fout << out.c_str();
	}
}