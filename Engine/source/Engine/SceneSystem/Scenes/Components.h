#pragma once

#include "Engine/UUID.h"

#include <glm/glm.hpp>

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
}