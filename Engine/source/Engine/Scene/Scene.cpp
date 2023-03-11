#include "epch.h"
#include "Scene.h"
#include "Entity.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/DirectX11/DX11Renderer.h"
#include "Engine/Physics/Physics.h"
#include "Engine/Platform/SDLInput.h"

namespace Engine
{

	Scene::~Scene()
	{
		
	}

	void Scene::InitScene()
	{
		m_MainCamera = std::make_unique<GameCamera>();
		m_MainCamera->SetPosition({ 0, 0, 50.0f });
		m_MainCamera->SetLens(0.25f * glm::pi<float>(), 16.0f / 9.0f, 1.0f, 1000.0f);
		//m_MainCamera->SetRotation({ 0.0f, 0.0f, 0.0f });

		Physics::Init();
	}

	void Scene::UnloadScene()
	{
		m_Registry.clear();
		m_SceneSettings = SceneSettings();
		Physics::ResetSimulation();
	}

	void Scene::RenderScene()
	{
		auto skyboxView = m_Registry.view<SkyboxComponent>();
		for (auto entityID : skyboxView) 
		{
			Entity entity{ entityID, shared_from_this() };
			Renderer::RendererEntity(entity);
		}


		auto MeshRenderers = m_Registry.view<MeshRendererComponent>();
		for (auto entityID : MeshRenderers)
		{
			Entity entity{ entityID, shared_from_this() };
			Renderer::RendererEntity(entity);
		}
	}

	void Scene::UpdateScene(float frametime)
	{
		m_MainCamera->Control(frametime);
		
	}

	void Scene::RemoveScene()
	{
	}

	void Scene::SimulateScene(float frametime)
	{
		Physics::Update(frametime);


		m_Registry.each([&](auto entityID)
			{
				Entity entity{ entityID, shared_from_this() };
				if (entity.HasComponent<RigidDynamicComponent>())
				{
					Physics::UpdatePhysicsActor(entity);
				}
			});
	}

	void Scene::EditorUpdatePhysicsScene(float frametime)
	{
		m_Registry.each([&](auto entityID)
			{
				Entity entity{ entityID, shared_from_this() };
				if (entity.HasComponent<RigidDynamicComponent>() || entity.HasComponent<CollisionComponents>())
				{
					Physics::EditorUpdateActors(entity);
				}
			});
	}

	/*void Scene::SetSkybox(std::string meshPath, std::string texPath, std::string assetPath)
	{
		if (!meshPath.empty())
		{
			std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(assetPath + "/" + meshPath);
			m_Skybox = std::make_shared<Model>(mesh);
		}

		std::shared_ptr<DX11Renderer> dx11Render = std::static_pointer_cast<DX11Renderer>(Renderer::GetRendererAPI());
		if (!texPath.empty())
		{
			CComPtr<ID3D11Resource> Resource;
			CComPtr<ID3D11ShaderResourceView> ResourceView;

			if (dx11Render->LoadTexture(assetPath + "/" + texPath, &Resource, &ResourceView))
			{
				SkyboxResourceView = ResourceView;
			}
		}
	}*/


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

	Entity Scene::CreateSkyboxEntity(const std::string& tag)
	{

		Entity entity = { m_Registry.create(), shared_from_this() };
		auto& ID = entity.AddComponent<IDComponent>();
		ID.ID = UUID();
		ID.Tag = tag;
		entity.AddComponent<TransformComponent>();
		entity.AddComponent<SkyboxComponent>();
		
		return entity;
	}

	void Scene::DeleteEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}

	void Scene::LoadEntities(std::string assetPath)
	{
		m_Registry.each([&](auto entityID)
			{
				Entity entity{ entityID, shared_from_this() };
				LoadEntity(entity, assetPath);
			});
	}

	void Scene::LoadEntity(Entity entity, std::string& assetPath)
	{
		if (entity.HasComponent<MeshRendererComponent>())
		{
			auto& comp = entity.GetComponent<MeshRendererComponent>();
			if (!comp.Path.empty())
			{
				std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(assetPath + "/" + comp.Path);
				comp.Model = std::make_shared<Model>(mesh);
			}
		}
		if (entity.HasComponent<TextureComponent>())
		{
			std::shared_ptr<DX11Renderer> dx11Render = std::static_pointer_cast<DX11Renderer>(Renderer::GetRendererAPI());
			auto& comp = entity.GetComponent<TextureComponent>();
			if (!comp.Path.empty())
			{
				CComPtr<ID3D11Resource> Resource;
				CComPtr<ID3D11ShaderResourceView> ResourceView;

				if (dx11Render->LoadTexture(assetPath + "/" + comp.Path, &Resource, &ResourceView))
				{
					comp.ResourceView = ResourceView;
				}
			}
			if (!comp.RoughPath.empty())
			{
				CComPtr<ID3D11Resource> Resource;
				CComPtr<ID3D11ShaderResourceView> ResourceView;

				if (dx11Render->LoadTexture(assetPath + "/" + comp.RoughPath, &Resource, &ResourceView))
				{
					comp.RoughView = ResourceView;
				}

			}
			if (!comp.NormalPath.empty())
			{
				CComPtr<ID3D11Resource> Resource;
				CComPtr<ID3D11ShaderResourceView> ResourceView;

				if (dx11Render->LoadTexture(assetPath + "/" + comp.NormalPath, &Resource, &ResourceView))
				{
					comp.NormalView = ResourceView;
				}

			}
			if (!comp.HeightPath.empty())
			{
				CComPtr<ID3D11Resource> Resource;
				CComPtr<ID3D11ShaderResourceView> ResourceView;

				if (dx11Render->LoadTexture(assetPath + "/" + comp.HeightPath, &Resource, &ResourceView))
				{
					comp.HeightView = ResourceView;
					
				}

			}
			if (!comp.MetalnessPath.empty())
			{
				CComPtr<ID3D11Resource> Resource;
				CComPtr<ID3D11ShaderResourceView> ResourceView;

				if (dx11Render->LoadTexture(assetPath + "/" + comp.MetalnessPath, &Resource, &ResourceView))
				{
					comp.MetalnessView = ResourceView;
					
				}

			}
		}
	}


	template<typename T>
	void Scene::OnComponentCreated(Entity entity, T& comp)
	{
		
	}

	template<>
	void Scene::OnComponentCreated<RigidDynamicComponent>(Entity entity, RigidDynamicComponent& comp)
	{
		Physics::CreatePhysicsActor(entity);
	}

	template<>
	void Scene::OnComponentCreated<CameraComponent>(Entity entity, CameraComponent& comp)
	{


	}

	template<>
	void Scene::OnComponentCreated<CollisionComponents>(Entity entity, CollisionComponents& comp)
	{
		Physics::CreateCollision(entity);
	}

	template<>
	void Scene::OnComponentCreated<ScriptComponent>(Entity entity, ScriptComponent& comp)
	{

	}

	template<>
	void Scene::OnComponentCreated<SkyboxComponent>(Entity entity, SkyboxComponent& comp)
	{
		if (entity.HasComponent<TransformComponent>()) 
		{
			auto& trans = entity.GetComponent<TransformComponent>();
			trans.Scale = { 50000.0f, 50000.0f, 50000.0f };
		}
	}

	
}