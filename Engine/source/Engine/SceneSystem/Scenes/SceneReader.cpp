#include "epch.h"
#include "SceneReader.h"


namespace YAML
{
	template<>
	struct convert<glm::vec3>
	{
		static bool decode(const Node& node, glm::vec3& vector)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			vector.x = node[0].as<float>();
			vector.y = node[1].as<float>();
			vector.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static bool decode(const Node& node, glm::vec4& vector)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			vector.x = node[0].as<float>();
			vector.y = node[1].as<float>();
			vector.z = node[2].as<float>();
			vector.w = node[3].as<float>();
			return true;
		}
	};
}

namespace Engine
{
	void SceneReader::Read(std::string& path, std::shared_ptr<Scene> scene)
	{
		YAML::Node data = YAML::LoadFile(path);
		auto SceneSettingsNode = data["SceneSettings"];
		if (SceneSettingsNode)
		{
			SceneSettings settings;
			settings.title = SceneSettingsNode["Title"].as<std::string>();
			settings.index = SceneSettingsNode["Index"].as<eint32>();
			settings.ambientColour = SceneSettingsNode["AmbientColour"].as<glm::vec3>();
			settings.specularPower = SceneSettingsNode["SpecularPower"].as<float>();
			settings.backgroundColour = SceneSettingsNode["BackgroundColour"].as<glm::vec4>();
			settings.vsyncOn = SceneSettingsNode["VSYNC"].as<bool>();
			scene->SetSceneSettings(settings);
		}
		
		auto EntitiesNode = data["Entities"];
		if (EntitiesNode)
		{
			//LOG_DEBUG(EntitiesNode);
			for (auto entityIT : EntitiesNode)
			{
				auto idNode = entityIT["IDComponent"];
				Entity entity = scene->CreateEntityWithUUID(idNode["ID"].as<eint64>(), idNode["Tag"].as<std::string>());

				auto transformNode = entityIT["TransformComponent"];
				if (transformNode)
				{
					auto& transform = entity.GetComponent<TransformComponent>();
					transform.Position = transformNode["Position"].as<glm::vec3>();
					transform.Rotation = transformNode["Rotation"].as<glm::vec3>();
					transform.Scale = transformNode["Scale"].as<glm::vec3>();
				}

				auto textureNode = entityIT["TextureComponent"];
				if (textureNode)
				{
					auto& texture = entity.AddComponent<TextureComponent>();
					texture.Path = textureNode["Path"].as<std::string>();
					texture.RoughPath = textureNode["RoughPath"].as<std::string>();
					texture.HeightPath = textureNode["HeightPath"].as<std::string>();
					texture.MetalnessPath = textureNode["MetalnessPath"].as<std::string>();
				}

				auto meshRendererNode = entityIT["MeshRendererComponent"];
				if (meshRendererNode)
				{
					auto& renderer = entity.AddComponent<MeshRendererComponent>();
					renderer.Path = meshRendererNode["Path"].as<std::string>();
					renderer.PixelShader = (EPixelShader)meshRendererNode["PixelShader"].as<int>();
					renderer.VertexShader = (EVertexShader)meshRendererNode["VertexShader"].as<int>();
					renderer.BlendState = (EBlendState)meshRendererNode["BlendState"].as<int>();
					renderer.DepthStencil = (EDepthStencilState)meshRendererNode["DepthStencil"].as<int>();
					renderer.RasterizerState = (ERasterizerState)meshRendererNode["Rasterizer"].as<int>();
					renderer.SamplerState = (ESamplerState)meshRendererNode["Sampler"].as<int>();
				}
			}
		}

	}

}