#pragma once

#include "Engine/UUID.h"

#include <glm/glm.hpp>

// Remove these includes
#include "Engine/Lab/Model.h"
#include <PxPhysicsAPI.h>

namespace Engine
{
	enum class EVertexShader
	{
		PixelLightingVertexShader = 0,
		BasicTransformVertexShader,
		SkinningVertexShader,
		PBRVertexShader,

		EVertexShaderSize
	};

	enum class EPixelShader
	{
		PixelLightingPixelShader = 0,
		LightModelPixelShader,
		PBRPixelShader,

		EPixelShaderSize
	};

	enum class ESamplerState
	{
		Anisotropic4xSampler = 0,
		TrilinearSampler,
		PointSampler,

		ESamplerStateSize
	};

	// Enum class for setting the Blend states
	enum class EBlendState
	{
		NoBlendingState = 0,
		AdditiveBlendingState,

		EBlendStateSize
	};

	// Enum class for setting the Rasterizer states
	enum class ERasterizerState
	{
		CullBackState = 0,
		CullFrontState,
		CullNoneState,

		ERasterizerStateSize
	};

	// Enum class for setting the Depth Stencil states
	enum class EDepthStencilState
	{
		UseDepthBufferState = 0,
		DepthReadOnlyState,
		NoDepthBufferState,

		EDepthStencilStateSize
	};

	enum class ECollisionTypes
	{
		Box = 0,
		Sphere,

		ECollisionTypesSize
	};

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

	struct RigidDynamicComponent
	{
		physx::PxRigidDynamic* actor;
		
	};

	struct RigidStaticComponent
	{
		physx::PxRigidStatic* actor;
	};

	// Need different colliison components
	struct CollisionComponents
	{ 
		ECollisionTypes CollisionType = ECollisionTypes::Box;
		physx::PxShape* CollisionShape;
	};

	struct ScriptComponent
	{
		// Create Scripts // Require scripting library
		std::string temp = "";
	};
}