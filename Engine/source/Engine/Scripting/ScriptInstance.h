#pragma once
#include "ScriptClass.h"

class _MonoClass;

namespace Engine
{
	class ScriptInstance
	{
	public:
		ScriptInstance(std::shared_ptr<ScriptClass> ScriptClass);

		void OnBegin();
		void OnUpdate(float deltaTime);

	private:
		std::shared_ptr<ScriptClass> m_ScriptClass;
	};

}
