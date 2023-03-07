#include "epch.h"
#include "Writers.h"

namespace YAML
{
	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& vector)
		{
			Node node;
			node.push_back(vector.x);
			node.push_back(vector.y);
			node.push_back(vector.z);
			return node;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& vector)
		{
			Node node;
			node.push_back(vector.x);
			node.push_back(vector.y);
			node.push_back(vector.z);
			node.push_back(vector.w);
			return node;
		}
	};
}

namespace Engine
{
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& vector)
	{
		out << YAML::Flow; // Keeps it on one line
		out << YAML::BeginSeq << vector.x << vector.y << vector.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& vector)
	{
		out << YAML::Flow; // Keeps it on one line
		out << YAML::BeginSeq << vector.x << vector.y << vector.z << vector.w << YAML::EndSeq;
		return out;
	}

	void SceneWriter::Write(std::string& path, std::shared_ptr<Scene> scene)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;

		std::filesystem::path scenePath = path;
		SceneSettings settings = scene->GetSceneSettings();
		//settings.title = scenePath.filename().string();
		settings.title = scenePath.filename().replace_extension("").string();
		scene->SetSceneSettings(settings);

		//scene->GetSceneSettings().title = scenePath.filename().string();
		SaveSceneSettings(out, scene);

		if (!scene->GetEntityRegistry().empty())
		{
			out << YAML::Key << "Entities" << YAML::BeginSeq;
			scene->GetEntityRegistry().each([&](auto entityID)
				{
					Entity entity{ entityID, scene };
					SaveEntity(out, entity);
				});

			out << YAML::EndSeq;
		}
		out << YAML::EndMap;

		std::ofstream fout(path);
		fout << out.c_str();
	}

	void SceneWriter::SaveSceneSettings(YAML::Emitter& out, std::shared_ptr<Scene> scene)
	{
		out << YAML::Key << "SceneSettings";
		out << YAML::BeginMap;
		out << YAML::Key << "Title" << YAML::Value << scene->GetSceneSettings().title;
		out << YAML::Key << "Index" << YAML::Value << scene->GetSceneSettings().index;
		out << YAML::Key << "AmbientColour" << YAML::Value << scene->GetSceneSettings().ambientColour;
		out << YAML::Key << "SpecularPower" << YAML::Value << scene->GetSceneSettings().specularPower;
		out << YAML::Key << "BackgroundColour" << YAML::Value << scene->GetSceneSettings().backgroundColour;
		out << YAML::Key << "VSYNC" << YAML::Value << scene->GetSceneSettings().vsyncOn;
		out << YAML::EndMap;
	}

	void SceneWriter::SaveEntity(YAML::Emitter& out, Entity entity)
	{

		out << YAML::BeginMap;
		out << YAML::Comment("Entity");
		if (entity.HasComponent<IDComponent>())
		{
			auto& comp = entity.GetComponent<IDComponent>();
			out << YAML::Key << "IDComponent";
			out << YAML::BeginMap;
			out << YAML::Key << "ID" << YAML::Value << comp.ID;
			out << YAML::Key << "Tag" << YAML::Value << comp.Tag;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<TransformComponent>())
		{
			auto& comp = entity.GetComponent<TransformComponent>();
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap;
			out << YAML::Key << "Position" << YAML::Value << comp.Position;
			out << YAML::Key << "Rotation" << YAML::Value << comp.Rotation;
			out << YAML::Key << "Scale" << YAML::Value << comp.Scale;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<TextureComponent>())
		{
			auto& comp = entity.GetComponent<TextureComponent>();
			out << YAML::Key << "TextureComponent";
			out << YAML::BeginMap;
			out << YAML::Key << "Path" << YAML::Value << comp.Path;
			out << YAML::Key << "NormalPath" << YAML::Value << comp.NormalPath;
			out << YAML::Key << "RoughPath" << YAML::Value << comp.RoughPath;
			out << YAML::Key << "HeightPath" << YAML::Value << comp.HeightPath;
			out << YAML::Key << "MetalnessPath" << YAML::Value << comp.MetalnessPath;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<MeshRendererComponent>())
		{
			auto& comp = entity.GetComponent<MeshRendererComponent>();
			out << YAML::Key << "MeshRendererComponent";
			out << YAML::BeginMap;
			out << YAML::Key << "Path" << YAML::Value << comp.Path;
			out << YAML::Key << "PixelShader" << YAML::Value << (int)comp.PixelShader;
			out << YAML::Key << "VertexShader" << YAML::Value << (int)comp.VertexShader;
			out << YAML::Key << "BlendState" << YAML::Value << (int)comp.BlendState;
			out << YAML::Key << "DepthStencil" << YAML::Value << (int)comp.DepthStencil;
			out << YAML::Key << "Rasterizer" << YAML::Value << (int)comp.RasterizerState;
			out << YAML::Key << "Sampler" << YAML::Value << (int)comp.SamplerState;


			out << YAML::EndMap;
		}
		if (entity.HasComponent<RigidDynamicComponent>())
		{
			auto& comp = entity.GetComponent<RigidDynamicComponent>();
			out << YAML::Key << "RigidDynamicComponent";
			out << YAML::BeginMap;
			out << YAML::Key << "Gravity" << YAML::Value << (bool)comp.Gravity;
			out << YAML::Key << "Mass" << YAML::Value << (float)comp.Mass;
			out << YAML::Key << "MassSpaceInertiaTensor" << YAML::Value << comp.MassSpaceInertiaTensor;
			out << YAML::Key << "LinearVelocity" << YAML::Value << comp.LinearVelocity;
			out << YAML::Key << "AngularVelocity" << YAML::Value << comp.AngularVelocity;
			out << YAML::Key << "AngularDamping" << YAML::Value << (float)comp.AngularDamping;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<CollisionComponents>())
		{
			auto& comp = entity.GetComponent<CollisionComponents>();
			out << YAML::Key << "CollisionComponents";
			out << YAML::BeginMap;
			out << YAML::Key << "CollisionType" << YAML::Value << (int)comp.CollisionType;
			
			out << YAML::EndMap;
		}

		out << YAML::EndMap;
	}

	void EditorSettingsWriter::Write(std::string& path, EditorSettings settings)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "EditorSettings";
		out << YAML::BeginMap;
		out << YAML::Key << "StartUpScene" << YAML::Value << settings.m_StartUpScene;
		out << YAML::EndMap;

		std::ofstream fout(path + "\\Settings.txt");
		fout << out.c_str();
	}
}