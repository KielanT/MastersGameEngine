#include "epch.h"
#include "DX11Renderer.h"
#include "Engine/Lab/GraphicsHelpers.h"

#include "imgui.h"
#include "backends/imgui_impl_dx11.h"
#include "backends/imgui_impl_sdl.h"

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

        // Create GPU-side constant buffers to receive the gPerFrameConstants and gPerModelConstants structures above
        // These allow us to pass data from CPU to shaders such as lighting information or matrices
        // See the comments above where these variable are declared and also the UpdateScene function
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

        return true;
	}

    void DX11Renderer::RenderLoop(std::shared_ptr<Scene> scene)
    {
        RenderScene(scene);
        RenderSceneFromCamera(scene);
    }

	void DX11Renderer::ShutdownRenderer()
	{
        if (m_D3DContext)
        {
            m_D3DContext->ClearState();
        }
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

    void DX11Renderer::RenderScene(std::shared_ptr<Scene> scene)
    {
        ImGui_ImplDX11_NewFrame();
        //ImGui_ImplWin32_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
        
        PerFrameConstants.ambientColour = scene->GetSceneSettings().ambientColour;
        PerFrameConstants.specularPower = scene->GetSceneSettings().specularPower;
        PerFrameConstants.cameraPosition = scene->GetCamera()->Position();
        
        m_D3DContext->OMSetRenderTargets(1, &m_BackBufferRenderTarget.p, m_DepthStencil);
        
        m_D3DContext->OMSetRenderTargets(1, &m_SceneRenderTarget.p, m_DepthStencil);
        
        
        // Clear the back buffer to a fixed colour and the depth buffer to the far distance
        //glm::vec4 backgroundColour = m_Scenes[m_SceneIndex]->GetBackgroundColour();
        glm::vec4 backgroundColour = scene->GetSceneSettings().backgroundColour;
        m_D3DContext->ClearRenderTargetView(m_BackBufferRenderTarget, &backgroundColour.r);
        m_D3DContext->ClearRenderTargetView(m_SceneRenderTarget, &backgroundColour.r);
        m_D3DContext->ClearDepthStencilView(m_DepthStencil, D3D11_CLEAR_DEPTH, 1.0f, 0);
        
        
        // Render the scene from the main camera
        RenderSceneFromCamera(scene);
        
        //Layer.RenderGUI();
        
        ImGui::Render();
        m_D3DContext->OMSetRenderTargets(1, &m_BackBufferRenderTarget.p, nullptr);
        
        //// Scene completion ////
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        
        //Layer.Update();
        
        // When drawing to the off-screen back buffer is complete, we "present" the image to the front buffer (the screen)
        // Set first parameter to 1 to lock to vsync (typically 60fps)
        //d11Renderer->GetSwapChain()->Present(m_Scenes[m_SceneIndex]->GetVSync() ? 1 : 0, 0);
        m_SwapChain->Present(scene->GetSceneSettings().vsyncOn ? 1 : 0, 0);

        

        ImGuiIO& io = ImGui::GetIO();

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }
    }

    void DX11Renderer::RenderSceneFromCamera(std::shared_ptr<Scene> scene)
    {
        PerFrameConstants.viewMatrix = scene->GetCamera()->ViewMatrix();
        PerFrameConstants.EngineionMatrix = scene->GetCamera()->EngineionMatrix();
        PerFrameConstants.viewEngineionMatrix = scene->GetCamera()->ViewEngineionMatrix();

        UpdateConstantBuffer(m_D3DContext, PerFrameConstantBuffer, PerFrameConstants);

        // Indicate that the constant buffer we just updated is for use in the vertex shader (VS) and pixel shader (PS)
        m_D3DContext->VSSetConstantBuffers(0, 1, &PerFrameConstantBuffer.p); // First parameter must match constant buffer number in the shader 
        m_D3DContext->PSSetConstantBuffers(0, 1, &PerFrameConstantBuffer.p);

        
    }

}