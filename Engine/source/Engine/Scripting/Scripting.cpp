#include "epch.h"
#include "Scripting.h"
#include "ScriptingAPI.h"

namespace Engine
{
	static std::unique_ptr<ScriptingAPI> m_ScriptingAPI = nullptr;

	bool Scripting::InitScripting()
	{
		m_ScriptingAPI = std::make_unique<ScriptingAPI>();
		
		return m_ScriptingAPI->InitScripting();
	}

	void Scripting::test()
	{
		m_ScriptingAPI->CallTestScript();
	}

	void Scripting::ShutdownScripting()
	{
		if (m_ScriptingAPI != nullptr)
		{
			m_ScriptingAPI->ShutdownScripting();
		}
	}

}