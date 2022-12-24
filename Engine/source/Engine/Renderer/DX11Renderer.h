#pragma once
#include "RendererAPI.h"

namespace Engine
{
	// TODO for Renderer
	// Create A swapchain creation function

	struct PerFrameConstants
	{
		// These are the matrices used to position the camera
		glm::mat4 viewMatrix;
		glm::mat4 EngineionMatrix;
		glm::mat4 viewEngineionMatrix; // The above two matrices multiplied together to combine their effects

		glm::vec3   light1Position; // 3 floats: x, y z
		float      padding1;       // Pad above variable to float4 (HLSL requirement - which we must duplicate in this the C++ version of the structure)
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

	// This is the matrix that positions the next thing to be rendered in the scene. Unlike the structure above this data can be
	// updated and sent to the GPU several times every frame (once per model). However, apart from that it works in the same way.

	static const int MAX_BONES = 64;
	struct PerModelConstants
	{
		glm::mat4 worldMatrix;
		glm::vec3   objectColour; // Allows each light model to be tinted to match the light colour they cast
		float      padding6;
		glm::mat4 boneMatrices[MAX_BONES]; /*** MISSING - fill in this array size - easy. Relates to another MISSING*/
	};

	class DX11Renderer : public RendererAPI
	{
	public:
		virtual bool Init(WindowProperties& props) override;

		virtual void RenderLoop(std::shared_ptr<Scene> scene) override;

		virtual void ShutdownRenderer() override;

		void InitiliseSceneTexture(WindowProperties& props);

	private:
		CComPtr<ID3D11Buffer> CreateConstantBuffer(int size);

		// Find better names for the functions below
		void RenderScene(std::shared_ptr<Scene> scene);
		void RenderSceneFromCamera(std::shared_ptr<Scene> scene);

	private:
		CComPtr<ID3D11Device> m_D3DDevice = nullptr; // D3D device for overall features
		CComPtr<ID3D11DeviceContext> m_D3DContext = nullptr; // D3D context for specific rendering tasks

		// Swap chain and back buffer
		CComPtr<IDXGISwapChain> m_SwapChain = nullptr;
		CComPtr<ID3D11RenderTargetView> m_BackBufferRenderTarget = nullptr;

		// Depth buffer (can also contain "stencil" values, which we will see later)
		CComPtr<ID3D11Texture2D> m_DepthStencilTexture = nullptr; // The texture holding the depth values
		CComPtr<ID3D11DepthStencilView> m_DepthStencil = nullptr; // The depth buffer referencing above texture

		CComPtr<ID3D11Texture2D> m_SceneTexture = nullptr;
		CComPtr<ID3D11RenderTargetView> m_SceneRenderTarget = nullptr;
		CComPtr<ID3D11ShaderResourceView> m_SceneTextureSRV = nullptr;


		PerFrameConstants PerFrameConstants; // Used for setting per frame constant variables and sending them to the GPU
		CComPtr<ID3D11Buffer> PerFrameConstantBuffer;

		PerModelConstants PerModelConstants;  // Used for setting per model constant variables and sending them to the GPU
		CComPtr<ID3D11Buffer> PerModelConstantBuffer;
	};
}

