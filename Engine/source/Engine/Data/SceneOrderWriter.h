#pragma once

#include "Engine/Scene/Scene.h"

#include <yaml-cpp/yaml.h>

namespace Engine
{
	class SceneOrderWriter
	{
	public:
		void Write(std::string& path, SceneOrder scene);
	};
}

