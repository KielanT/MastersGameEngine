#include "epch.h"
#include "DX11Renderer.h"
#include "Engine/Lab/GraphicsHelpers.h"
#include "Engine/Scene/Entity.h"
#include "Engine/Layer.h"

#include "imgui.h"
#include "backends/imgui_impl_SDL.h"
#include "backends/imgui_impl_dx11.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Engine
{
	bool DX11Renderer::Init(WindowProperties& props)
	{
        HRESULT hr = S_OK;

        //// Initialise DirectX ////

        // Create a Direct3D device (i.e. initialise D3D) and create a swap-chain (create a back buffer to render to)
        DXGI_SWAP_CHAIN_DESC swapDesc = {};
        swapDesc.OutputWindow = props.Hwnd;                           // Target window
        swapDesc.Windowed = TRUE;
        swapDesc.BufferCount = 1;
        swapDesc.BufferDesc.Width = props.Width;             // Target window size
        swapDesc.BufferDesc.Height = props.Height;            // --"--
        swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // Pixel format of target window
        swapDesc.BufferDesc.RefreshRate.Numerator = 60;        // Refresh rate of monitor (provided as fraction = 60/1 here)
        swapDesc.BufferDesc.RefreshRate.Denominator = 1;         // --"--
        swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapDesc.SampleDesc.Count = 1;
        swapDesc.SampleDesc.Quality = 0;
        UINT flags = 0;// D3D11_CREATE_DEVICE_DEBUG; // Set this to D3D11_CREATE_DEVICE_DEBUG to get more debugging information (in the "Output" window of Visual Studio)
        hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, flags, 0, 0, D3D11_SDK_VERSION,
            &swapDesc, &m_SwapChain, &m_D3DDevice, nullptr, &m_D3DContext);
        if (FAILED(hr))
        {
            LOG_ERROR("Error creating Direct3D device");
            return false;
        }


        // Get a "render target view" of back-buffer - standard behaviour
        CComPtr<ID3D11Texture2D> backBuffer;
        hr = m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
        if (FAILED(hr))
        {
            LOG_ERROR("Error creating swap chain");
            return false;
        }
        hr = m_D3DDevice->CreateRenderTargetView(backBuffer, NULL, &m_BackBufferRenderTarget);
        //backBuffer->Release();
        if (FAILED(hr))
        {
            LOG_ERROR("Error creating render target view");
            return false;
        }


        //// Create depth buffer to go along with the back buffer ////

        // First create a texture to hold the depth buffer values
        D3D11_TEXTURE2D_DESC dbDesc = {};
        dbDesc.Width = props.Width; // Same size as viewport / back-buffer
        dbDesc.Height = props.Height;
        dbDesc.MipLevels = 1;
        dbDesc.ArraySize = 1;
        dbDesc.Format = DXGI_FORMAT_D32_FLOAT; // Each depth value is a single float
        dbDesc.SampleDesc.Count = 1;
        dbDesc.SampleDesc.Quality = 0;
        dbDesc.Usage = D3D11_USAGE_DEFAULT;
        dbDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        dbDesc.CPUAccessFlags = 0;
        dbDesc.MiscFlags = 0;
        hr = m_D3DDevice->CreateTexture2D(&dbDesc, nullptr, &m_DepthStencilTexture);
        if (FAILED(hr))
        {
            LOG_ERROR("Error creating depth buffer texture");
            return false;
        }

        // Create the depth stencil view - an object to allow us to use the texture
        // just created as a depth buffer
        D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
        dsvDesc.Format = dbDesc.Format;
        dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
        dsvDesc.Texture2D.MipSlice = 0;
        hr = m_D3DDevice->CreateDepthStencilView(m_DepthStencilTexture, &dsvDesc,
            &m_DepthStencil);
        if (FAILED(hr))
        {
            LOG_ERROR("Error creating depth buffer view");
            return false;
        }

        PerFrameConstantBuffer = CreateConstantBuffer(sizeof(PerFrameConstants));
        PerModelConstantBuffer = CreateConstantBuffer(sizeof(PerModelConstants));
        if (PerFrameConstantBuffer == nullptr || PerModelConstantBuffer == nullptr)
        {
            LOG_ERROR("Error creating constant buffers");
            return false;
        }

        D3D11_VIEWPORT vp;
        vp.Width = static_cast<FLOAT>(props.Width);
        vp.Height = static_cast<FLOAT>(props.Height);
        vp.MinDepth = 0.0f;
        vp.MaxDepth = 1.0f;
        vp.TopLeftX = 0;
        vp.TopLeftY = 0;
        m_D3DContext->RSSetViewports(1, &vp);

        InitiliseSceneTexture(props);

        InitGUI();
        
        m_Props = props;

        m_Shader = std::make_shared<DX11Shader>();
        if (!m_Shader->InitShaders())
        {
            LOG_ERROR("Failed to inilisied shaders");
            return false;
        }

        m_States = std::make_shared<DX11States>();
        if (!m_States->InitStates())
        {
            LOG_ERROR("Failed to inilisied states");
            return false;
        }

        return true;
	}

    void DX11Renderer::RenderLoop()
    {
        RenderScene();
        Layer::Render();
    }

    void DX11Renderer::Renderer(Entity entity)
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

                m_D3DContext->VSSetShader(m_Shader->GetVertexShader(), nullptr, 0);
                m_D3DContext->PSSetShader(m_Shader->GetPixelShader(), nullptr, 0);
                
                m_D3DContext->PSSetShaderResources(0, 1, &texture.ResourceView.p);
                m_D3DContext->PSSetShaderResources(1, 1, &texture.RoughView.p);
                m_D3DContext->PSSetShaderResources(2, 1, &texture.NormalView.p);
                m_D3DContext->PSSetShaderResources(3, 1, &texture.HeightView.p);
                m_D3DContext->PSSetShaderResources(4, 1, &texture.MetalnessView.p);
                
                CComPtr<ID3D11SamplerState> sampler =  m_States->GetSamplerState(mesh.SamplerState);
                m_D3DContext->PSSetSamplers(0, 1, &sampler.p);
                
                m_D3DContext->OMSetBlendState(m_States->GetBlendState(mesh.BlendState), nullptr, 0xffffff);
                m_D3DContext->OMSetDepthStencilState(m_States->GetDepthStencilState(mesh.DepthStencil), 0);
                m_D3DContext->RSSetState(m_States->GetRasterizerState(mesh.RasterizerState));
  
                entity.GetComponent<MeshRendererComponent>().Model->Render();
            }
        }
    }

    void DX11Renderer::Present()
    {
        if(m_Scene != nullptr)
            m_SwapChain->Present(m_Scene->GetSceneSettings().vsyncOn ? 1 : 0, 0);
        else
            m_SwapChain->Present(true, 0);
    }

    void DX11Renderer::GUINewFrame()
    {
        if (ImGui::GetCurrentContext() != nullptr)
            ImGui_ImplDX11_NewFrame();
    }

    void DX11Renderer::GUIRenderDrawData()
    {
        if (ImGui::GetCurrentContext() != nullptr)
            ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }

    void DX11Renderer::ShutdownRenderer()
	{
        if (m_D3DContext)
        {
            m_D3DContext->ClearState();
        }

        if (ImGui::GetCurrentContext() != nullptr)
            ImGui_ImplDX11_Shutdown();
	}

    void DX11Renderer::InitGUI()
    {
        if (ImGui::GetCurrentContext() != nullptr)
            ImGui_ImplDX11_Init(m_D3DDevice, m_D3DContext);
    }

    CComPtr<ID3D11ShaderResourceView> DX11Renderer::GetSceneTexture()
    {
        return m_SceneTextureSRV;
    }

    void DX11Renderer::InitiliseSceneTexture(WindowProperties& props)
    {
        D3D11_TEXTURE2D_DESC sceneTextureDesc = {};

        sceneTextureDesc.Width = props.Width;  // Full-screen post-processing - use full screen size for texture
        sceneTextureDesc.Height = props.Height;
        sceneTextureDesc.MipLevels = 1; // No mip-maps when rendering to textures (or we would have to render every level)
        sceneTextureDesc.ArraySize = 1;
        sceneTextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // RGBA texture (8-bits each)
        sceneTextureDesc.SampleDesc.Count = 1;
        sceneTextureDesc.SampleDesc.Quality = 0;
        sceneTextureDesc.Usage = D3D11_USAGE_DEFAULT;
        sceneTextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE; // IMPORTANT: Indicate we will use texture as render target, and pass it to shaders
        sceneTextureDesc.CPUAccessFlags = 0;
        sceneTextureDesc.MiscFlags = 0;
        if (FAILED(m_D3DDevice->CreateTexture2D(&sceneTextureDesc, NULL, &m_SceneTexture)))
        {
            LOG_ERROR("Error Creating Scene Texture");
            //return false;
        }

        if (FAILED(m_D3DDevice->CreateRenderTargetView(m_SceneTexture, NULL, &m_SceneRenderTarget)))
        {
            LOG_ERROR("Error Creating Scene Render Target View");
            //return false;
        }

        D3D11_SHADER_RESOURCE_VIEW_DESC srDesc = {};
        srDesc.Format = sceneTextureDesc.Format;
        srDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        srDesc.Texture2D.MostDetailedMip = 0;
        srDesc.Texture2D.MipLevels = 1;
        if (FAILED(m_D3DDevice->CreateShaderResourceView(m_SceneTexture, &srDesc, &m_SceneTextureSRV)))
        {
            LOG_ERROR("Error Creating Scene Shader Resource View");
            //return false;
        }
    }

    bool DX11Renderer::LoadTextureFromFile(const char* filename, ID3D11ShaderResourceView** out_srv, eint32* out_width, eint32* out_height)
    {
        // Code From https://github.com/ocornut/imgui/wiki/Image-Loading-and-Displaying-Examples
        int image_width = 0;
        int image_height = 0;
        unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
        if (image_data == NULL)
        {
            return false;
            LOG_ERROR("Image Data is Null");
        }
        // Create texture
        D3D11_TEXTURE2D_DESC desc;
        ZeroMemory(&desc, sizeof(desc));
        desc.Width = image_width;
        desc.Height = image_height;
        desc.MipLevels = 1;
        desc.ArraySize = 1;
        desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.SampleDesc.Count = 1;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        desc.CPUAccessFlags = 0;

        ID3D11Texture2D* pTexture = NULL;
        D3D11_SUBRESOURCE_DATA subResource;
        subResource.pSysMem = image_data;
        subResource.SysMemPitch = desc.Width * 4;
        subResource.SysMemSlicePitch = 0;
        m_D3DDevice->CreateTexture2D(&desc, &subResource, &pTexture);

        // Create texture view
        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
        ZeroMemory(&srvDesc, sizeof(srvDesc));
        srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MipLevels = desc.MipLevels;
        srvDesc.Texture2D.MostDetailedMip = 0;
        m_D3DDevice->CreateShaderResourceView(pTexture, &srvDesc, out_srv);
        pTexture->Release();


        *out_width = image_width;
        *out_height = image_height;
        stbi_image_free(image_data);

        return true;
    }

    bool DX11Renderer::LoadTexture(std::string filename, ID3D11Resource** texture, ID3D11ShaderResourceView** textureSRV)
    {
        std::string dds = ".dds"; // So check the filename extension (case insensitive)
        if (filename.size() >= 4 &&
            std::equal(dds.rbegin(), dds.rend(), filename.rbegin(), [](unsigned char a, unsigned char b) { return std::tolower(a) == std::tolower(b); }))
        {
            return SUCCEEDED(DirectX::CreateDDSTextureFromFile(m_D3DDevice, CA2CT(filename.c_str()), texture, textureSRV));
        }
        else
        {
            return SUCCEEDED(DirectX::CreateWICTextureFromFile(m_D3DDevice, m_D3DContext, CA2CT(filename.c_str()), texture, textureSRV));
        }
    }

    CComPtr<ID3D11Buffer> DX11Renderer::CreateConstantBuffer(int size)
    {
        D3D11_BUFFER_DESC cbDesc;
        cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        cbDesc.ByteWidth = 16 * ((size + 15) / 16);     // Constant buffer size must be a multiple of 16 - this maths rounds up to the nearest multiple
        cbDesc.Usage = D3D11_USAGE_DYNAMIC;             // Indicates that the buffer is frequently updated
        cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // CPU is only going to write to the constants (not read them)
        cbDesc.MiscFlags = 0;
        CComPtr<ID3D11Buffer> constantBuffer;
        HRESULT hr = m_D3DDevice->CreateBuffer(&cbDesc, nullptr, &constantBuffer);
        if (FAILED(hr))
        {
            LOG_ERROR("Error Creating Constant Buffers");
            return nullptr;
        }

        return constantBuffer;
    }

    void DX11Renderer::RenderScene()
    {
        static glm::vec4 bgColour = glm::vec4(0.2f, 0.2f, 0.3f, 1.0f);
        if (m_Scene != nullptr && m_Scene->GetCamera() != nullptr)
        {
            PerFrameConstants.ambientColour = m_Scene->GetSceneSettings().ambientColour;
            PerFrameConstants.specularPower = m_Scene->GetSceneSettings().specularPower;
            PerFrameConstants.cameraPosition = m_Scene->GetCamera()->GetPosition();
            bgColour = m_Scene->GetSceneSettings().backgroundColour;
        }

        m_D3DContext->OMSetRenderTargets(1, &m_BackBufferRenderTarget.p, m_DepthStencil);

        m_D3DContext->OMSetRenderTargets(1, &m_SceneRenderTarget.p, m_DepthStencil);
        
        m_D3DContext->ClearRenderTargetView(m_BackBufferRenderTarget, &bgColour.r);
        m_D3DContext->ClearRenderTargetView(m_SceneRenderTarget, &bgColour.r);
        m_D3DContext->ClearDepthStencilView(m_DepthStencil, D3D11_CLEAR_DEPTH, 1.0f, 0);

        // Render the scene from the main camera
        RenderSceneFromCamera();

        m_D3DContext->OMSetRenderTargets(1, &m_BackBufferRenderTarget.p, nullptr);

    }

    void DX11Renderer::RenderSceneFromCamera()
    {
        if (m_Scene != nullptr && m_Scene->GetCamera() != nullptr)
        {
            PerFrameConstants.viewMatrix = m_Scene->GetCamera()->GetView();
            PerFrameConstants.EngineionMatrix = m_Scene->GetCamera()->GetProj();
            PerFrameConstants.viewEngineionMatrix = m_Scene->GetCamera()->GetView() * m_Scene->GetCamera()->GetProj();
        }
        UpdateConstantBuffer(m_D3DContext, PerFrameConstantBuffer, PerFrameConstants);

        // Indicate that the constant buffer we just updated is for use in the vertex shader (VS) and pixel shader (PS)
        m_D3DContext->VSSetConstantBuffers(0, 1, &PerFrameConstantBuffer.p); // First parameter must match constant buffer number in the shader 
        m_D3DContext->PSSetConstantBuffers(0, 1, &PerFrameConstantBuffer.p);

        if (m_Scene != nullptr)
        {
            m_Scene->RenderScene();
        }
    }

}