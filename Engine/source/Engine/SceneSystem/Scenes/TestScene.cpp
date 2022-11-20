#include "epch.h"
#include "TestScene.h"
#include "Engine/Renderer/DirectX11/DirectX11Renderer.h"
#include "Engine/Renderer/DirectX11/DirectX11Shader.h"
#include "Engine/Renderer/DirectX11/DirectX11States.h"
#include "Engine/Lab/GraphicsHelpers.h"



namespace Engine
{
    TestScene::TestScene(DirectX11SceneManager* sceneManager, IRenderer* renderer, int sceneIndex, glm::vec3 ambientColour, float specularPower, glm::vec4 backgroundColour, bool vsyncOn)
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

        DirectX11Renderer* renderer = static_cast<DirectX11Renderer*>(m_Renderer);
        Mesh* mesh = new Mesh(renderer, path + "Cube.x");
        model = new Model(mesh);
        
        m_SceneCamera = new Camera();

        return true;
    }

    bool TestScene::InitScene()
    {
        model->SetPosition({ 0.0f, 0.0f, 0.0f });
        
        m_SceneCamera->SetPosition({ 0, 0, -50 });
        m_SceneCamera->SetRotation({ 0.0f, 0.0f, 0.0f });

        CreateEntity("c");
        CreateEntity("b");
        CreateEntity("a");

        return true;
    }

    void TestScene::RenderScene()
    {
        DirectX11Renderer* dx11Renderer = static_cast<DirectX11Renderer*>(m_Renderer);
        DirectX11Shader* shader = static_cast<DirectX11Shader*>(m_Shader);
        DirectX11States* state = static_cast<DirectX11States*>(m_State);

        dx11Renderer->GetDeviceContext()->VSSetShader(shader->GetVertexShader(EVertexShader::PixelLightingVertexShader), nullptr, 0);
        dx11Renderer->GetDeviceContext()->PSSetShader(shader->GetPixelShader(EPixelShader::PixelLightingPixelShader), nullptr, 0);

        // Select the approriate textures and sampler to use in the pixel shader
        ID3D11Resource* resource;
        ID3D11ShaderResourceView* resourceView;
        
        if (!LoadTexture(dx11Renderer, "media/CargoA.dds", &resource, &resourceView))
        {
            LOG_ERROR("Cannot load texture");
        }
        ID3D11SamplerState* sampler = state->GetSamplerState(ESamplerState::Anisotropic4xSampler);
        dx11Renderer->GetDeviceContext()->PSSetShaderResources(0, 1, &resourceView); // First parameter must match texture slot number in the shader
        dx11Renderer->GetDeviceContext()->PSSetSamplers(0, 1, &sampler);
        
        // States - no blending, normal depth buffer and culling
        dx11Renderer->GetDeviceContext()->OMSetBlendState(state->GetBlendState(EBlendState::NoBlendingState), nullptr, 0xffffff);
        dx11Renderer->GetDeviceContext()->OMSetDepthStencilState(state->GetDepthStencilState(EDepthStencilState::UseDepthBufferState), 0);
        dx11Renderer->GetDeviceContext()->RSSetState(state->GetRasterizerState(ERasterizerState::CullNoneState));
        model->Render();
    }

    void TestScene::UpdateScene(float frameTime)
    {
        float roty = model->Rotation().x;
        roty += 0.5f * frameTime;
		model->SetRotation({ roty, 0.0f,  0.0f});

        float a;

        m_SceneCamera->Control(frameTime);
       
    }

    void TestScene::ReleaseResources()
    {
        if (m_SceneCamera != nullptr) delete m_SceneCamera;

    }

    Entity TestScene::CreateEntity(const std::string& tag)
    {
        Entity entity = { m_Registry.create(), this };
        auto& ID = entity.AddComponent<IDComponent>();
        ID.ID = UUID();
        ID.Tag = tag;
        entity.AddComponent<TransformComponent>();
        return entity;
    }
}