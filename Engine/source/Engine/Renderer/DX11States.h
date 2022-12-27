#pragma once

namespace Engine
{

	enum class EDX11SamplerState
	{
		Anisotropic4xSampler = 0,
		TrilinearSampler,
		PointSampler,

		ESamplerStateSize
	};

	// Enum class for setting the Blend states
	enum class EDX11BlendState
	{
		NoBlendingState = 0,
		AdditiveBlendingState,

		EBlendStateSize
	};

	// Enum class for setting the Rasterizer states
	enum class EDX11RasterizerState
	{
		CullBackState = 0,
		CullFrontState,
		CullNoneState,

		ERasterizerStateSize
	};

	// Enum class for setting the Depth Stencil states
	enum class EDX11DepthStencilState
	{
		UseDepthBufferState = 0,
		DepthReadOnlyState,
		NoDepthBufferState,

		EDepthStencilStateSize
	};

	class DX11States
	{
	public:
		bool InitStates();

		CComPtr<ID3D11SamplerState> GetSamplerState(EDX11SamplerState ss); // Returns the Sampler state 
		CComPtr<ID3D11BlendState> GetBlendState(EDX11BlendState bs); // Returns the Blend state 
		CComPtr<ID3D11RasterizerState> GetRasterizerState(EDX11RasterizerState rs); // Returns the Rasterizer state 
		CComPtr<ID3D11DepthStencilState> GetDepthStencilState(EDX11DepthStencilState dss); // Returns the Depth Stencil state 

	private:
		// Member variables
		CComPtr<ID3D11SamplerState> m_PointSampler; // Variable for the Point sampler
		CComPtr<ID3D11SamplerState> m_TrilinearSampler; // Variable for the Trilinear sampler
		CComPtr<ID3D11SamplerState> m_Anisotropic4xSampler; // Variable for the Anisotropic4x sampler

		CComPtr<ID3D11BlendState> m_NoBlendingState;  // Variable for the No Blending state
		CComPtr<ID3D11BlendState> m_AdditiveBlendingState; // Variable for the Additive Blending state

		CComPtr<ID3D11RasterizerState> m_CullBackState; // Variable for the Cull Back state
		CComPtr<ID3D11RasterizerState> m_CullFrontState; // Variable for the Cull Front state
		CComPtr<ID3D11RasterizerState> m_CullNoneState; // Variable for the Cull None state

		CComPtr<ID3D11DepthStencilState> m_UseDepthBufferState; // Variable for the Use Depth Buffer state
		CComPtr<ID3D11DepthStencilState> m_DepthReadOnlyState; // Variable for the Depth Read Only state
		CComPtr<ID3D11DepthStencilState> m_NoDepthBufferState; // Variable for the No Depth Buffer state
	};
}
