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
		Physics::Shutdown();
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


		Physics::Init();
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
		Physics::Update(frametime);

		

		static bool test = false;

		if (SDLInput::KeyHeld(SDLK_f))
		{
			test = true;
		}


		m_Registry.each([&](auto entityID)
			{
				Entity entity{ entityID, shared_from_this() };
				if (entity.HasComponent<MeshRendererComponent>())
				{
					Physics::TempTestFunction(entity, test);
				}
			});
		
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

	void Scene::LoadEntities()
	{
		m_Registry.each([&](auto entityID)
			{
				Entity entity{ entityID, shared_from_this() };
				LoadEntity(entity);
			});
	}

	void Scene::LoadEntity(Entity entity)
	{
		if (entity.HasComponent<MeshRendererComponent>())
		{
			auto& comp = entity.GetComponent<MeshRendererComponent>();
			if (!comp.Path.empty())
			{
				std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(comp.Path);
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

				if (dx11Render->LoadTexture(comp.Path, &Resource, &ResourceView))
				{
					comp.ResourceView = ResourceView;
				}
			}
			if (!comp.RoughPath.empty())
			{
				CComPtr<ID3D11Resource> Resource;
				CComPtr<ID3D11ShaderResourceView> ResourceView;

				if (dx11Render->LoadTexture(comp.RoughPath, &Resource, &ResourceView))
				{
					comp.RoughView = ResourceView;
				}

			}
			if (!comp.NormalPath.empty())
			{
				CComPtr<ID3D11Resource> Resource;
				CComPtr<ID3D11ShaderResourceView> ResourceView;

				if (dx11Render->LoadTexture(comp.NormalPath, &Resource, &ResourceView))
				{
					comp.NormalView = ResourceView;
				}

			}
			if (!comp.HeightPath.empty())
			{
				CComPtr<ID3D11Resource> Resource;
				CComPtr<ID3D11ShaderResourceView> ResourceView;

				if (dx11Render->LoadTexture(comp.HeightPath, &Resource, &ResourceView))
				{
					comp.HeightView = ResourceView;
					
				}

			}
			if (!comp.MetalnessPath.empty())
			{
				CComPtr<ID3D11Resource> Resource;
				CComPtr<ID3D11ShaderResourceView> ResourceView;

				if (dx11Render->LoadTexture(comp.MetalnessPath, &Resource, &ResourceView))
				{
					comp.MetalnessView = ResourceView;
					
				}

			}
		}
	}

}