#include "epch.h"
#include "Scene.h"
#include "Entity.h"

namespace Engine
{
	Scene::Scene(std::shared_ptr<IRenderer> renderer) : m_Renderer(renderer)
	{
	}

	Scene::~Scene()
	{
	}

	void Scene::InitScene()
	{
		m_MainCamera = std::make_unique<Camera>();
		m_MainCamera->SetPosition({ 0, 0, -50 });
		m_MainCamera->SetRotation({ 0.0f, 0.0f, 0.0f });

		m_Shader = SetShader(m_Renderer->GetRendererType());
		m_Shader->InitShaders(m_Renderer);

		m_State = SetStates(m_Renderer->GetRendererType());
		m_State->InitStates(m_Renderer);

		std::string path = "media/";

		std::filesystem::path MainPath = std::filesystem::current_path();

		std::filesystem::path meshPath = std::filesystem::current_path().parent_path().append("Engine\\");

		std::filesystem::current_path(meshPath); // Sets the current path to the mesh path
	}

	void Scene::RenderScene()
	{
		m_Registry.each([&](auto entityID)
			{
				Entity entity{ entityID, shared_from_this() };
				if (entity.HasComponent<MeshRendererComponent>())
				{
					Renderer(entity);
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

	void Scene::Renderer(Entity entity)
	{
		if (entity.HasComponent<MeshRendererComponent>() && entity.HasComponent<TextureComponent>())
		{
			auto transfrom = entity.GetComponent<TransformComponent>();
			auto mesh = entity.GetComponent<MeshRendererComponent>();
			auto texture = entity.GetComponent<TextureComponent>();

			if (mesh.Model != nullptr)
			{
				mesh.Model->SetPosition(transfrom.Position);
				mesh.Model->SetRotation(transfrom.Rotation);
				mesh.Model->SetScale(transfrom.Scale);

				std::shared_ptr<DirectX11Renderer> dx11Renderer = std::static_pointer_cast<DirectX11Renderer>(m_Renderer);
				std::shared_ptr<DirectX11Shader> shader = std::static_pointer_cast<DirectX11Shader>(m_Shader);
				std::shared_ptr<DirectX11States> state = std::static_pointer_cast<DirectX11States>(m_State);

				dx11Renderer->GetDeviceContext()->VSSetShader(shader->GetVertexShader(mesh.VertexShader), nullptr, 0);
				dx11Renderer->GetDeviceContext()->PSSetShader(shader->GetPixelShader(mesh.PixelShader), nullptr, 0);

				dx11Renderer->GetDeviceContext()->PSSetShaderResources(0, 1, &texture.ResourceView.p);
				dx11Renderer->GetDeviceContext()->PSSetShaderResources(1, 1, &texture.RoughView.p);
				dx11Renderer->GetDeviceContext()->PSSetShaderResources(2, 1, &texture.NormalView.p);
				dx11Renderer->GetDeviceContext()->PSSetShaderResources(3, 1, &texture.HeightView.p);
				dx11Renderer->GetDeviceContext()->PSSetShaderResources(4, 1, &texture.MetalnessView.p);

				CComPtr<ID3D11SamplerState> sampler = state->GetSamplerState(mesh.SamplerState);
				dx11Renderer->GetDeviceContext()->PSSetSamplers(0, 1, &sampler.p);

				dx11Renderer->GetDeviceContext()->OMSetBlendState(state->GetBlendState(mesh.BlendState), nullptr, 0xffffff);
				dx11Renderer->GetDeviceContext()->OMSetDepthStencilState(state->GetDepthStencilState(mesh.DepthStencil), 0);
				dx11Renderer->GetDeviceContext()->RSSetState(state->GetRasterizerState(mesh.RasterizerState));

				entity.GetComponent<MeshRendererComponent>().Model->Render();
			}
		}
	}
}