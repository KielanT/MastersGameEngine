#pragma once
#include "Engine/Scene/Scene.h"
#include "Engine/Scripting/Scripting.h"
#include "Engine/Scene/Entity.h"
#include "mono/metadata/object.h"
#include "Engine/Scene/Components.h"


static void Transform_GetPostition(Engine::UUID entityID, glm::vec3* position)
{
	std::shared_ptr<Engine::Scene> scene = Engine::Scripting::GetInstance()->GetScene();
	if (scene != nullptr)
	{
		Engine::Entity entity = scene->FindEntityByUUID(entityID);
		auto& transform = entity.GetComponent<Engine::TransformComponent>();
		*position = transform.Position;
	}
}

static void Transform_SetPostition(Engine::UUID entityID, glm::vec3* position)
{
	std::shared_ptr<Engine::Scene> scene = Engine::Scripting::GetInstance()->GetScene();
	if (scene != nullptr)
	{
		Engine::Entity entity = scene->FindEntityByUUID(entityID);

		if (entity.HasComponent<Engine::RigidDynamicComponent>())
		{
			auto& comp = entity.GetComponent<Engine::RigidDynamicComponent>();
			glm::vec3 pos = *position;
			auto physxTrans = physx::PxTransform({ pos.x , pos.y, pos.z });
			comp.actor->setGlobalPose(physxTrans);
		}
		else if (entity.HasComponent<Engine::CollisionComponents>())
		{
			auto& comp = entity.GetComponent<Engine::CollisionComponents>();
			glm::vec3 pos = *position;
			auto physxTrans = physx::PxTransform({ pos.x , pos.y, pos.z });
			comp.actor->setGlobalPose(physxTrans);
		}
		else
		{
			auto& transform = entity.GetComponent<Engine::TransformComponent>();
			transform.Position = *position;
		}
	}
}

static void Transform_GetRotation(Engine::UUID entityID, glm::vec3* rot)
{
	std::shared_ptr<Engine::Scene> scene = Engine::Scripting::GetInstance()->GetScene();
	if (scene != nullptr)
	{
		Engine::Entity entity = scene->FindEntityByUUID(entityID);
		auto& transform = entity.GetComponent<Engine::TransformComponent>();
		*rot = transform.Rotation;
	}
}

static void Transform_SetRotation(Engine::UUID entityID, glm::vec3* rot)
{
	std::shared_ptr<Engine::Scene> scene = Engine::Scripting::GetInstance()->GetScene();
	if (scene != nullptr)
	{
		Engine::Entity entity = scene->FindEntityByUUID(entityID);
		auto& transform = entity.GetComponent<Engine::TransformComponent>();
		transform.Rotation = *rot;
	}
}

static void Transform_GetScale(Engine::UUID entityID, glm::vec3* scale)
{
	std::shared_ptr<Engine::Scene> scene = Engine::Scripting::GetInstance()->GetScene();
	if (scene != nullptr)
	{
		Engine::Entity entity = scene->FindEntityByUUID(entityID);
		auto& transform = entity.GetComponent<Engine::TransformComponent>();
		*scale = transform.Scale;
	}
}

static void Transform_SetScale(Engine::UUID entityID, glm::vec3* scale)
{
	std::shared_ptr<Engine::Scene> scene = Engine::Scripting::GetInstance()->GetScene();
	if (scene != nullptr)
	{
		Engine::Entity entity = scene->FindEntityByUUID(entityID);
		auto& transform = entity.GetComponent<Engine::TransformComponent>();
		transform.Scale = *scale;
	}
}

static void Renderer_SetVisible(Engine::UUID entityID, bool* visible)
{
	std::shared_ptr<Engine::Scene> scene = Engine::Scripting::GetInstance()->GetScene();
	if (scene != nullptr)
	{
		Engine::Entity entity = scene->FindEntityByUUID(entityID);
		if (entity.HasComponent<Engine::MeshRendererComponent>())
		{
			auto& renderer = entity.GetComponent<Engine::MeshRendererComponent>();
			renderer.bIsVisible = *visible;
		}
	}
}

static void Renderer_GetVisible(Engine::UUID entityID, bool* visible)
{
	std::shared_ptr<Engine::Scene> scene = Engine::Scripting::GetInstance()->GetScene();
	if (scene != nullptr)
	{
		Engine::Entity entity = scene->FindEntityByUUID(entityID);
		if (entity.HasComponent<Engine::MeshRendererComponent>())
		{
			auto& renderer = entity.GetComponent<Engine::MeshRendererComponent>();
			*visible = renderer.bIsVisible;
		}
	}
}

static void Physics_AddForce(Engine::UUID entityID, physx::PxVec3 force)
{
	std::shared_ptr<Engine::Scene> scene = Engine::Scripting::GetInstance()->GetScene();
	if (scene != nullptr)
	{
		Engine::Entity entity = scene->FindEntityByUUID(entityID);
		if (entity.HasComponent<Engine::RigidDynamicComponent>())
		{
			auto& rdc = entity.GetComponent<Engine::RigidDynamicComponent>();
			rdc.actor->addForce(force);
		}
	}
}
