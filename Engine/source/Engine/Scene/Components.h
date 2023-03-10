#pragma once

#include "Engine/UUID.h"

#include <glm/glm.hpp>

// Remove these includes
#include "Engine/Lab/Model.h"
#include <PxPhysicsAPI.h>

namespace Engine
{


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
		EBlendState BlendState = EBlendState::NoBlendingState; // BlendState
		EDepthStencilState DepthStencil = EDepthStencilState::UseDepthBufferState; // Depeth Stencil State
		ERasterizerState RasterizerState = ERasterizerState::CullNoneState; // Rasterize State
		ESamplerState SamplerState = ESamplerState::Anisotropic4xSampler; // Sampler State
	};

	struct CameraComponent
	{
		// Create Camera;
		std::string temp = ""; // Requires at least one var
		bool IsActive;
	};

	struct RigidDynamicComponent
	{
		physx::PxRigidDynamic* actor;
		
		// Gravity on/off
		bool Gravity = false;

		//Mass;
		float Mass = 1.0f;
		glm::vec3 MassSpaceInertiaTensor = glm::vec3(1.0f, 1.0f, 1.0f);

		//velocity
		glm::vec3 LinearVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 AngularVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
		float AngularDamping = 0.0f;
	
	};


	struct CollisionComponents
	{ 
		physx::PxRigidStatic* actor = nullptr; // Only gets set if the there isn't a rigid dynamic actor
		ECollisionTypes CollisionType = ECollisionTypes::Box;
		physx::PxShape* CollisionShape;

		glm::vec3 BoxBounds = glm::vec3(5.0f, 5.0f, 5.0f);
		float SphereRadius = 5.0f;

		struct SPhysicsMaterial
		{
			float staticFriction = 0.5f;
			float dynamicFriction = 0.5f;
			float restitution = 0.6f;
		};

		SPhysicsMaterial PhysicsMaterial;
	};

	struct ScriptComponent
	{
		// Create Scripts // Require scripting library
		std::string temp = "";
	};

	struct SkyboxComponent
	{
		std::string MeshPath = ""; // Mesh Path
		std::shared_ptr<Model> Model;

		std::string TexPath = "";
		CComPtr<ID3D11ShaderResourceView> ResourceView;

	};
}