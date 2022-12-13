#include "epch.h"
#include "TestScene.h"
#include "Engine/Renderer/DirectX11/DirectX11Renderer.h"
#include "Engine/Renderer/DirectX11/DirectX11Shader.h"
#include "Engine/Renderer/DirectX11/DirectX11States.h"
#include "Engine/Lab/GraphicsHelpers.h"



namespace Engine
{
    TestScene::TestScene(DirectX11SceneManager* sceneManager, std::shared_ptr<IRenderer> renderer, int sceneIndex, glm::vec3 ambientColour, float specularPower, glm::vec4 backgroundColour, bool vsyncOn)
    {
        m_Renderer = renderer;
        m_SceneIndex = sceneIndex;

        m_AmbientColour = ambientColour;
        m_SpecularPower = specularPower;
        m_backgroundColour = backgroundColour;
        m_VsyncOn = vsyncOn;

        m_sceneManager = sceneManager;
    }


    bool TestScene::InitGeometry()
    {
		
        m_Shader = SetShader(m_Renderer->GetRendererType());
        m_Shader->InitShaders(m_Renderer);

        m_State = SetStates(m_Renderer->GetRendererType());
        m_State->InitStates(m_Renderer);
		
        std::string path = "media/";
		
        std::filesystem::path MainPath = std::filesystem::current_path();
        
        std::filesystem::path meshPath = std::filesystem::current_path().parent_path().append("Engine\\");
        
        std::filesystem::current_path(meshPath); // Sets the current path to the mesh path
        
        m_SceneCamera = std::make_unique<Camera>();

        return true;
    }

    bool TestScene::InitScene()
    {
        
        m_SceneCamera->SetPosition({ 0, 0, -50 });
        m_SceneCamera->SetRotation({ 0.0f, 0.0f, 0.0f });

        return true;
    }

    void TestScene::RenderScene()
    {
        m_Registry.each([&](auto entityID)
            {
                Entity entity{ entityID, shared_from_this()};
                if (entity.HasComponent<MeshRendererComponent>())
                {
                    Renderer(entity);
                }
            });
    }

    void TestScene::UpdateScene(float frameTime)
    {

        m_SceneCamera->Control(frameTime);
       
    }

    void TestScene::ReleaseResources()
    {

    }

    Entity TestScene::CreateEntity(const std::string& tag)
    {
        Entity entity = { m_Registry.create(), shared_from_this()};
        auto& ID = entity.AddComponent<IDComponent>();
        ID.ID = UUID();
        ID.Tag = tag;
        entity.AddComponent<TransformComponent>();
        return entity;
    }

    Entity TestScene::CreateMeshEntity(const std::string& tag)
    {
        Entity entity = { m_Registry.create(), shared_from_this()};
        auto& ID = entity.AddComponent<IDComponent>();
        ID.ID = UUID();
        ID.Tag = tag;
        entity.AddComponent<TransformComponent>();
        entity.AddComponent<MeshRendererComponent>();
        entity.AddComponent<TextureComponent>();
        return entity;
    }

    void TestScene::Renderer(Entity entity)
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