#include "epch.h"
#include "ScriptInstance.h"

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"



namespace Engine
{
	ScriptInstance::ScriptInstance(std::shared_ptr<ScriptClass> ScriptClass)
	{
		if (ScriptClass != nullptr)
		{
			m_ScriptClass = ScriptClass;
			m_ScriptClass->InitClassInstance();
		}
	}

	void ScriptInstance::OnBegin(UUID id)
	{
		if (m_ScriptClass != nullptr)
		{
			m_ScriptClass->OnBegin(id);
		}
	}

	void ScriptInstance::OnUpdate(float deltaTime)
	{
		if (m_ScriptClass != nullptr)
		{
			m_ScriptClass->OnUpdate(deltaTime);
		}
	}

	void ScriptInstance::OnContact(UUID id)
	{
		if (m_ScriptClass != nullptr)
		{
			m_ScriptClass->OnContact(id);
		}
	}

	void ScriptInstance::ChangeScriptClass(std::shared_ptr<ScriptClass> newScriptClass)
	{
		m_ScriptClass = newScriptClass;
		m_ScriptClass->InitClassInstance();
	}

	const std::string ScriptInstance::GetScriptClassName()
	{
		return m_ScriptClass->GetMonoClassName();
	}

	std::shared_ptr<ScriptClass> ScriptInstance::GetScriptClass()
	{
		return m_ScriptClass;
	}

}