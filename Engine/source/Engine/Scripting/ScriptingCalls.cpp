#include "epch.h"
#include "ScriptingCalls.h"

#include "CPPBinds/UtilityBinds.h"

namespace Engine
{

	void ScriptingCalls::RegisterScriptFunctions()
	{
		mono_add_internal_call("Engine.InternalCalls::Utility_Log_Debug", Utility_Log_Debug);
		mono_add_internal_call("Engine.InternalCalls::Utility_Log_Error", Utility_Log_Error);
		mono_add_internal_call("Engine.InternalCalls::Utility_Log_Info", Utility_Log_Info);
		mono_add_internal_call("Engine.InternalCalls::Utility_Log_Warn", Utility_Log_Warn);
	}

	

}