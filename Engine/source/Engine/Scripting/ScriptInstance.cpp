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

	void ScriptInstance::OnBegin()
	{
		if (m_ScriptClass != nullptr)
		{
			m_ScriptClass->OnBegin();
		}
	}

	void ScriptInstance::OnUpdate(float deltaTime)
	{
		if (m_ScriptClass != nullptr)
		{
			m_ScriptClass->OnUpdate(deltaTime);
		}
	}

}