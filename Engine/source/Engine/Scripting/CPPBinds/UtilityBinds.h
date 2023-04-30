#pragma once
#include "Engine/Scene/Scene.h"
#include "Engine/Scripting/Scripting.h"
#include "Engine/Scene/Entity.h"
#include "mono/metadata/object.h"


static void Utility_Log_Debug(MonoString* message)
{
	MonoError error;
	std::string messageStr(mono_string_to_utf8_checked(message, &error));
	LOG_DEBUG(messageStr);
}

static void Utility_Log_Error(MonoString* message)
{
	MonoError error;
	std::string messageStr(mono_string_to_utf8_checked(message, &error));
	LOG_ERROR(messageStr);
}

static void Utility_Log_Info(MonoString* message)
{
	MonoError error;
	std::string messageStr(mono_string_to_utf8_checked(message, &error));
	LOG_INFO(messageStr);
}

static void Utility_Log_Warn(MonoString* message)
{
	MonoError error;
	std::string messageStr(mono_string_to_utf8_checked(message, &error));
	LOG_WARN(messageStr);
}

static uint64_t Entity_FindEntityByName(MonoString* name)
{
	std::shared_ptr<Engine::Scene> scene = Engine::Scripting::GetInstance()->GetScene();
	if (scene != nullptr)
	{
		std::string nameStr(mono_string_to_utf8(name));
		Engine::Entity entity = scene->FindEntityByName(nameStr);

		if (!entity)
			return 0;

		return entity.GetUUID();
	}
}

static uint64_t Entity_CreateEntityByCopy(uint64_t id, glm::vec3* position)
{
	std::shared_ptr<Engine::Scene> scene = Engine::Scripting::GetInstance()->GetScene();
	if (scene != nullptr)
	{
		Engine::Entity entity = scene->CreateEntityByCopy(id, *position);
		if (!entity)
			return 0;

		Engine::Scripting::GetInstance()->OnBeginEntity(entity);

		return entity.GetUUID();
	}
}