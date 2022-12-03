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

        std::shared_ptr<DirectX11Renderer> renderer = std::static_pointer_cast<DirectX11Renderer>(m_Renderer);
        mesh = std::make_shared<Mesh>(renderer, path + "Cube.x");
        model = std::make_shared<Model>(mesh);
        
        m_SceneCamera = std::make_unique<Camera>();

        if (!LoadTexture(renderer, "media/CargoA.dds", &resource, &resourceView))
        {
            LOG_ERROR("Cannot load texture");
        }

        return true;
    }

    bool TestScene::InitScene()
    {
        model->SetPosition({ 0.0f, 0.0f, 0.0f });
        
        m_SceneCamera->SetPosition({ 0, 0, -50 });
        m_SceneCamera->SetRotation({ 0.0f, 0.0f, 0.0f });

        TempEntity = CreateMeshEntity("TempEntity");

        return true;
    }

    void TestScene::RenderScene()
    {
        std::shared_ptr<DirectX11Renderer> dx11Renderer = std::static_pointer_cast<DirectX11Renderer>(m_Renderer);
        std::shared_ptr<DirectX11Shader> shader = std::static_pointer_cast<DirectX11Shader>(m_Shader);
        std::shared_ptr<DirectX11States> state = std::static_pointer_cast<DirectX11States>(m_State);



        EVertexShader vs = EVertexShader::PixelLightingVertexShader;
        EPixelShader ps = EPixelShader::PixelLightingPixelShader;
        EBlendState bs = EBlendState::NoBlendingState;
        EDepthStencilState dss = EDepthStencilState::UseDepthBufferState;
        ERasterizerState rs = ERasterizerState::CullNoneState;
        ESamplerState ss = ESamplerState::Anisotropic4xSampler;

        // TODO: Remove TempEntity
        if (TempEntity.HasComponent<MeshRendererComponent>())
        {
            vs = TempEntity.GetComponent<MeshRendererComponent>().VertexShader;
            ps = TempEntity.GetComponent<MeshRendererComponent>().PixelShader;
            bs = TempEntity.GetComponent<MeshRendererComponent>().BlendState;
            dss = TempEntity.GetComponent<MeshRendererComponent>().DepthStencil;
            rs = TempEntity.GetComponent<MeshRendererComponent>().RasterizerState;
            ss = TempEntity.GetComponent<MeshRendererComponent>().SamplerState;
        }
      

        dx11Renderer->GetDeviceContext()->VSSetShader(shader->GetVertexShader(vs), nullptr, 0);
        dx11Renderer->GetDeviceContext()->PSSetShader(shader->GetPixelShader(ps), nullptr, 0);

        CComPtr<ID3D11SamplerState> sampler = state->GetSamplerState(ss);

        if (TempEntity.HasComponent<TextureComponent>() && TempEntity.GetComponent<TextureComponent>().ResourceView != nullptr)
            dx11Renderer->GetDeviceContext()->PSSetShaderResources(0, 1, &TempEntity.GetComponent<TextureComponent>().ResourceView.p);
        else
            dx11Renderer->GetDeviceContext()->PSSetShaderResources(0, 1, &resourceView.p); // First parameter must match texture slot number in the shader
        
        dx11Renderer->GetDeviceContext()->PSSetSamplers(0, 1, &sampler.p);
        
        // States - no blending, normal depth buffer and culling
        dx11Renderer->GetDeviceContext()->OMSetBlendState(state->GetBlendState(bs), nullptr, 0xffffff);
        dx11Renderer->GetDeviceContext()->OMSetDepthStencilState(state->GetDepthStencilState(dss), 0);
        dx11Renderer->GetDeviceContext()->RSSetState(state->GetRasterizerState(rs));
        
        if (TempEntity.HasComponent<MeshRendererComponent>() && TempEntity.GetComponent<MeshRendererComponent>().Model != nullptr)
        {
            TempEntity.GetComponent<MeshRendererComponent>().Model->Render();
        }
        else
        {
            model->Render();
        }
      
    }

    void TestScene::UpdateScene(float frameTime)
    {
        float roty = model->Rotation().x;
        roty += 0.5f * frameTime;
		model->SetRotation({ roty, 0.0f,  0.0f});

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
}