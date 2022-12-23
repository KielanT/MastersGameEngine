#pragma once

#include "Engine/UUID.h"

#include <glm/glm.hpp>

#include "Engine/Renderer/DirectX11/DirectX11Shader.h"
#include "Engine/Renderer/DirectX11/DirectX11States.h"
#include "Engine/Lab/Model.h"

namespace Engine
{
	struct IDComponent
	{
		UUID ID;
		std::string Tag;
	};

	struct TransformComponent
	{
		glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };
	};

	struct TextureComponent
	{
		std::string Path = "";
		CComPtr<ID3D11ShaderResourceView> ResourceView;

		std::string RoughPath = "";
		CComPtr<ID3D11ShaderResourceView> RoughView;

		std::string NormalPath = "";
		CComPtr<ID3D11ShaderResourceView> NormalView;

		std::string HeightPath = "";
		CComPtr<ID3D11ShaderResourceView> HeightView;

		std::string MetalnessPath = "";
		CComPtr<ID3D11ShaderResourceView> MetalnessView;
	};

	struct MeshRendererComponent
	{
		std::string Path = ""; // Mesh Path
		std::shared_ptr<Model> Model;
		EPixelShader PixelShader = EPixelShader::PixelLightingPixelShader; // PixelShader
		EVertexShader VertexShader = EVertexShader::PixelLightingVertexShader; // VertexShader
		EBlendState BlendState = EBlendState::NoBlendingState; // BlendState
		EDepthStencilState DepthStencil = EDepthStencilState::UseDepthBufferState; // Depeth Stencil State
		ERasterizerState RasterizerState = ERasterizerState::CullNoneState; // Rasterize State
		ESamplerState SamplerState = ESamplerState::Anisotropic4xSampler; // Sampler State
	};

	struct CameraComponent
	{
		// Create Camera;
		std::string temp = ""; // Requires at least one var
	};

	// Physics Components will need to be split up in different components
	struct PhysicsComponents
	{
		// CreatePhysicsComponents  // Requires physic Library
		std::string temp = "";
	};

	// Need different colliison components
	struct CollisionComponents
	{
		// Create collisions // Requires physic Library
		std::string temp = "";
	};

	struct ScriptComponent
	{
		// Create Scripts // Require scripting library
		std::string temp = "";
	};
}