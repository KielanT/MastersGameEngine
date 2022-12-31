#include "epch.h"
#include "Scene.h"
#include "Entity.h"
#include "Engine/Renderer/Renderer.h"

namespace Engine
{

	Scene::~Scene()
	{
	}

	void Scene::InitScene()
	{
		m_MainCamera = std::make_unique<Camera>();
		m_MainCamera->SetPosition({ 0, 0, -50 });
		m_MainCamera->SetRotation({ 0.0f, 0.0f, 0.0f });

		std::string path = "media/";

		std::filesystem::path MainPath = std::filesystem::current_path();

		std::filesystem::path meshPath = std::filesystem::current_path().parent_path().append("Engine\\");

		std::filesystem::current_path(meshPath); // Sets the current path to the mesh path
	}

	void Scene::UnloadScene()
	{
		m_Registry.clear();
		m_SceneSettings = SceneSettings();
	}

	void Scene::RenderScene()
	{
		m_Registry.each([&](auto entityID)
			{
				Entity entity{ entityID, shared_from_this() };
				if (entity.HasComponent<MeshRendererComponent>())
				{
					Renderer::RendererEntity(entity);
				}
			});
	}

	void Scene::UpdateScene(float frametime)
	{
		m_MainCamera->Control(frametime);
	}

	void Scene::RemoveScene()
	{
	}

	Entity Scene::CreateEntity(const std::string& tag)
	{
		Entity entity = { m_Registry.create(), shared_from_this() };
		auto& ID = entity.AddComponent<IDComponent>();
		ID.ID = UUID();
		ID.Tag = tag;
		entity.AddComponent<TransformComponent>();
		return entity;
	}

	Entity Scene::CreateEntityWithUUID(UUID uuid, const std::string& tag)
	{
		Entity entity = { m_Registry.create(), shared_from_this() };
		auto& ID = entity.AddComponent<IDComponent>();
		ID.ID = uuid;
		ID.Tag = tag;
		entity.AddComponent<TransformComponent>();
		return entity;
	}

	Entity Scene::CreateMeshEntity(const std::string& tag)
	{
		Entity entity = { m_Registry.create(), shared_from_this() };
		auto& ID = entity.AddComponent<IDComponent>();
		ID.ID = UUID();
		ID.Tag = tag;
		entity.AddComponent<TransformComponent>();
		entity.AddComponent<MeshRendererComponent>();
		entity.AddComponent<TextureComponent>();
		return entity; 
	}

	void Scene::DeleteEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}
}