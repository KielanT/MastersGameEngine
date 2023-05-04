#pragma once
#include "ScriptClass.h"
#include "Engine/UUID.h"

class _MonoClass;

namespace Engine
{
	// Scripting instance used to store the class and an instance of the class to call the correct function
	class ScriptInstance
	{
	public:
		ScriptInstance(std::shared_ptr<ScriptClass> ScriptClass);

		void OnBegin(UUID id);
		void OnUpdate(float deltaTime);
		void OnContact(UUID id);

		void ChangeScriptClass(std::shared_ptr<ScriptClass> newScriptClass);

		const std::string GetScriptClassName();

		std::shared_ptr<ScriptClass> GetScriptClass();

	private:
		std::shared_ptr<ScriptClass> m_ScriptClass;
	};

}
