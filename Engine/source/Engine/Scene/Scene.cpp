#include "epch.h"
#include "Scene.h"
#include "Entity.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/DirectX11/DX11Renderer.h"
#include "Engine/Physics/Physics.h"
#include "Engine/Platform/SDLInput.h"
#include "GameCamera.h"
#include "Engine/Scripting/Scripting.h"

namespace Engine
{

	Scene::~Scene()
	{
		
	}

	void Scene::InitScene()
	{
		m_MainCamera = std::make_shared<GameCamera>();
		m_MainCamera->SetPosition({ 0, 0, 50.0f });
		m_MainCamera->SetLens(0.25f * glm::pi<float>(), 1600.0f / 900.0f, 1.0f, 1000.0f);
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
			Renderer::SetSkyboxEntity(entity);
			Renderer::RendererEntity(entity);
			break; // Only is one
		}


		auto MeshRenderers = m_Registry.view<MeshRendererComponent>();
		for (auto entityID : MeshRenderers)
		{
			Entity entity{ entityID, shared_from_this() };
			
			Renderer::RendererEntity(entity);
		}
	}

	void Scene::BeginScene()
	{
		auto scriptView = m_Registry.view<ScriptComponent>();
		for (auto entityID : scriptView)
		{
			Entity entity{ entityID, shared_from_this() };
			Scripting::GetInstance()->SetScene(shared_from_this());
			Scripting::GetInstance()->OnBeginEntity(entity);
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
		auto scriptView = m_Registry.view<ScriptComponent>();
		for (auto entityID : scriptView)
		{
			Entity entity{ entityID, shared_from_this() };
			Scripting::GetInstance()->OnUpdateEntity(entity, frametime);
		}
		
		
		Physics::Update(frametime);

		auto RDView = m_Registry.view<RigidDynamicComponent>();
		for (auto entityID : RDView)
		{
			Entity entity{ entityID, shared_from_this() };
			Physics::UpdatePhysicsActor(entity);
		}
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

	void Scene::SetActiveCamera()
	{
		auto CamView = m_Registry.view<CameraComponent>();
		for (auto entityID : CamView)
		{
			Entity entity{ entityID, shared_from_this() };
			m_MainCamera = entity.GetComponent<CameraComponent>().Camera;
		}
	}

	Entity Scene::FindEntityByName(const std::string& name)
	{
		auto IDView = m_Registry.view<IDComponent>();
		for (auto entityID : IDView)
		{
			const auto& id = IDView.get<IDComponent>(entityID);
			if (id.Tag == name)
				return Entity{ entityID, shared_from_this() };
		}

		return {};
	}

	Entity Scene::FindEntityByUUID(UUID id)
	{
		auto IDView = m_Registry.view<IDComponent>();
		for (auto entityID : IDView)
		{
			const auto& uuid = IDView.get<IDComponent>(entityID);
			if (uuid.ID == id)
				return Entity{ entityID, shared_from_this() };
		}

		return {};
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

		if (entity.HasComponent<SkyboxComponent>()) 
		{
			auto& comp = entity.GetComponent<SkyboxComponent>();
			if (!comp.MeshPath.empty()) 
			{
				std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(assetPath + "/" + comp.MeshPath);
				comp.Model = std::make_shared<Model>(mesh);
			}

			std::shared_ptr<DX11Renderer> dx11Render = std::static_pointer_cast<DX11Renderer>(Renderer::GetRendererAPI());
			if (!comp.TexPath.empty())
			{
				CComPtr<ID3D11Resource> Resource;
				CComPtr<ID3D11ShaderResourceView> ResourceView;

				if (dx11Render->LoadTexture(assetPath + "/" + comp.TexPath, &Resource, &ResourceView))
				{
					comp.TexMapView = ResourceView;
				}
			}
		}

		if (entity.HasComponent<CameraComponent>())
		{
			auto& comp = entity.GetComponent<CameraComponent>();
			comp.Camera = std::make_shared<GameCamera>();
			comp.Camera->SetLens(0.25f * glm::pi<float>(), 1600.0f / 900.0f, 1.0f, 1000.0f);
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
		comp.Camera = std::make_shared<GameCamera>();
		comp.Camera->SetLens(0.25f * glm::pi<float>(), 1600.0f / 900.0f, 1.0f, 1000.0f);
	}

	template<>
	void Scene::OnComponentCreated<CollisionComponents>(Entity entity, CollisionComponents& comp)
	{
		Physics::CreateCollision(entity);
	}

	template<>
	void Scene::OnComponentCreated<ScriptComponent>(Entity entity, ScriptComponent& comp)
	{
		comp.OwnerEntityId = entity.GetUUID();
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