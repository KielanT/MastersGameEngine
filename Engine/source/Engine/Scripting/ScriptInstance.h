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

		void ChangeScriptClass(std::shared_ptr<ScriptClass> newScriptClass);

		const std::string GetScriptClassName();

		std::shared_ptr<ScriptClass> GetScriptClass();

	private:
		std::shared_ptr<ScriptClass> m_ScriptClass;
	};

}
