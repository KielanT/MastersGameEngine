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

		//m_MainCamera = std::make_unique<Camera>();
		//m_MainCamera->SetPosition({ 0, 0, -50.0f });
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

		if (m_MainCamera != nullptr)
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

	void Scene::EditorUpdateScene(float frametime)
	{
		if (m_MainCamera != nullptr)
			m_MainCamera->Control(frametime);


		m_Registry.each([&](auto entityID)
			{
				Entity entity{ entityID, shared_from_this() };
				if (entity.HasComponent<RigidDynamicComponent>() || entity.HasComponent<CollisionComponents>())
				{
					Physics::EditorUpdateActors(entity);
				}
			});
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

	Entity Scene::CreateCameraEntity(const std::string& tag)
	{
		Entity entity = { m_Registry.create(), shared_from_this() };
		auto& ID = entity.AddComponent<IDComponent>();
		ID.ID = UUID();
		ID.Tag = tag;
		entity.AddComponent<TransformComponent>();
		entity.AddComponent<CameraComponent>();

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

	void Scene::FindActiveCamera()
	{
		m_Registry.each([&](auto entityID)
			{
				Entity entity{ entityID, shared_from_this() };
				{
					
					if (entity.HasComponent<CameraComponent>()) 
					{
						
						if (entity.GetComponent<CameraComponent>().IsActive) 
						{
							LOG_INFO("Set Camera");
							m_MainCamera = entity.GetComponent<CameraComponent>().Camera;
						}
					}
				}
			});
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
		comp.Camera = std::make_shared<Camera>();

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

	
}