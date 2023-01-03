#include "Engine/Scene/Scene.h"

namespace Engine
{
	class SceneOrderReader
	{
	public:
		void Read(std::string& path, SceneOrder& scene);
	};
}