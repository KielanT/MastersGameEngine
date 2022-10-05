#pragma once
#include "Engine/Interfaces/IRenderer.h"

struct PerFrameConstants
{
	glm::mat4 viewMatrix;
	glm::mat4 EngineionMatrix;
	glm::mat4 viewEngineionMatrix; 

	glm::vec3   light1Position;
	float      padding1;       
	glm::vec3   light1Colour;
	float      padding2;

	glm::vec3   light2Position;
	float      padding3;
	glm::vec3   light2Colour;
	float      padding4;

	glm::vec3   ambientColour;
	float      specularPower;

	glm::vec3   cameraPosition;
	float      padding5;
};


static const int MAX_BONES = 64;
struct PerModelConstants
{
	glm::mat4 worldMatrix;
	glm::vec3   objectColour; 
	float      padding6;
	glm::mat4 boneMatrices[MAX_BONES];
};

namespace Engine
{
	class DirectX11Renderer : public IRenderer
	{
	public:
		DirectX11Renderer() = default;
		virtual ~DirectX11Renderer();

		DirectX11Renderer(const DirectX11Renderer&) = delete;
		DirectX11Renderer(DirectX11Renderer&&) = delete;
		DirectX11Renderer& operator=(const DirectX11Renderer&) = delete;
		DirectX11Renderer& operator=(DirectX11Renderer&&) = delete;

	public:
		virtual bool Init(WindowProperties& props) override;

		// Shutdown the renderer
		virtual void Shutdown() override;

		// Get the renderer type
		virtual const ERendererType GetRendererType() override { return ERendererType::DirectX11; }

		// Get the Window Properties
		virtual WindowProperties GetWindowProperties() override { return m_Props; }

	public:
		CComPtr<ID3D11Device> GetDevice() { return m_D3DDevice; } // Returns the DirectX11 device
		CComPtr<ID3D11DeviceContext> GetDeviceContext() { return m_D3DContext; } // Returns the DirectX11 device context
		CComPtr<IDXGISwapChain> GetSwapChain() { return m_SwapChain; } // Returns the DirectX11 Swapchain
		CComPtr<ID3D11RenderTargetView> GetBackBuffer() { return m_BackBufferRenderTarget; } // Returns the DirectX11 BackBuffer
		CComPtr<ID3D11DepthStencilView> GetDepthStencil() { return m_DepthStencil; } // Returns the DirectX11 Depth Stencil
		
		CComPtr<ID3D11Buffer> CreateConstantBuffer(int size); // Function used for creating a constant buffer

	public:
		PerFrameConstants PerFrameConstants; // Used for setting per frame constant variables and sending them to the GPU
		CComPtr<ID3D11Buffer> PerFrameConstantBuffer;

		PerModelConstants PerModelConstants;  // Used for setting per model constant variables and sending them to the GPU
		CComPtr<ID3D11Buffer> PerModelConstantBuffer;


	private:
		// The main Direct3D (D3D) variables
		CComPtr<ID3D11Device> m_D3DDevice; // D3D device for overall features
		CComPtr<ID3D11DeviceContext> m_D3DContext; // D3D context for specific rendering tasks

		// Swap chain and back buffer
		CComPtr<IDXGISwapChain> m_SwapChain;
		CComPtr<ID3D11RenderTargetView> m_BackBufferRenderTarget;

		// Depth buffer (can also contain "stencil" values, which we will see later)
		CComPtr<ID3D11Texture2D> m_DepthStencilTexture; // The texture holding the depth values
		CComPtr<ID3D11DepthStencilView> m_DepthStencil; // The depth buffer referencing above texture

		WindowProperties m_Props; // Used for getting the window properties

		
	};
}
