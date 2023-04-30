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
		auto& transform = entity.GetComponent<Engine::TransformComponent>();
		transform.Position = *position;
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