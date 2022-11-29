#include "epch.h"
#include "DirectX11Renderer.h"

namespace Engine
{
    DirectX11Renderer::~DirectX11Renderer()
    {
        ShutdownRenderer();
    }

    bool DirectX11Renderer::InitRenderer(WindowProperties& props)
    {
        HRESULT hr = S_OK;

        m_Props = props;

        //// Initialise DirectX ////

        // Create a Direct3D device (i.e. initialise D3D) and create a swap-chain (create a back buffer to render to)
        DXGI_SWAP_CHAIN_DESC swapDesc = {};
        swapDesc.OutputWindow = m_Props.Hwnd;                           // Target window
        swapDesc.Windowed = TRUE;
        swapDesc.BufferCount = 1;
        swapDesc.BufferDesc.Width = m_Props.Width;             // Target window size
        swapDesc.BufferDesc.Height = m_Props.Height;            // --"--
        swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // Pixel format of target window
        swapDesc.BufferDesc.RefreshRate.Numerator = 60;        // Refresh rate of monitor (provided as fraction = 60/1 here)
        swapDesc.BufferDesc.RefreshRate.Denominator = 1;         // --"--
        swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapDesc.SampleDesc.Count = 1;
        swapDesc.SampleDesc.Quality = 0;
        UINT flags = 0; // Set this to D3D11_CREATE_DEVICE_DEBUG to get more debugging information (in the "Output" window of Visual Studio)
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
        dbDesc.Width = m_Props.Width; // Same size as viewport / back-buffer
        dbDesc.Height = m_Props.Height;
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
        vp.Width = static_cast<FLOAT>(m_Props.Width);
        vp.Height = static_cast<FLOAT>(m_Props.Height);
        vp.MinDepth = 0.0f;
        vp.MaxDepth = 1.0f;
        vp.TopLeftX = 0;
        vp.TopLeftY = 0;
        m_D3DContext->RSSetViewports(1, &vp);

        InitiliseSceneTexture();

        return true;
    }

    void DirectX11Renderer::InitiliseSceneTexture()
    {
		D3D11_TEXTURE2D_DESC sceneTextureDesc = {};
		
        sceneTextureDesc.Width = m_Props.Width;  // Full-screen post-processing - use full screen size for texture
        sceneTextureDesc.Height = m_Props.Height;
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

    void DirectX11Renderer::ShutdownRenderer()
    {
        if (m_D3DContext)
        {
            m_D3DContext->ClearState();
        }

    }
	
    void DirectX11Renderer::Resize(eint32 height, eint32 width)
    {
        CComPtr<ID3D11RenderTargetView> rtv = nullptr;
        m_D3DContext->OMSetRenderTargets(1, &rtv, nullptr);

        m_Props.Width = width;
        m_Props.Height = height;

        m_SwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
		
        m_SceneTexture = nullptr;
        m_SceneRenderTarget = nullptr;
        m_SceneTextureSRV = nullptr;

        m_BackBufferRenderTarget = nullptr;

        CComPtr<ID3D11Texture2D> backBuffer;
        m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
        m_D3DDevice->CreateRenderTargetView(backBuffer, NULL, &m_BackBufferRenderTarget);
        //backBuffer->Release();
        


        D3D11_TEXTURE2D_DESC sceneTextureDesc = {};
        sceneTextureDesc.Width = m_Props.Width;  // Full-screen post-processing - use full screen size for texture
        sceneTextureDesc.Height = m_Props.Height;
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

        D3D11_VIEWPORT vp;
        vp.Width = static_cast<FLOAT>(m_Props.Width);
        vp.Height = static_cast<FLOAT>(m_Props.Height);
        vp.MinDepth = 0.0f;
        vp.MaxDepth = 1.0f;
        vp.TopLeftX = 0;
        vp.TopLeftY = 0;
        m_D3DContext->RSSetViewports(1, &vp);
		
    }
	
    CComPtr<ID3D11Buffer> DirectX11Renderer::CreateConstantBuffer(int size)
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
}