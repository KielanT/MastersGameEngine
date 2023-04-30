#include "epch.h"
#include "ScriptingCalls.h"

#include "CPPBinds/UtilityBinds.h"
#include "CPPBinds/ComponentBinds.h"

namespace Engine
{

	void ScriptingCalls::RegisterScriptFunctions()
	{
		// Utilities
		mono_add_internal_call("Engine.InternalCalls::Utility_Log_Debug", Utility_Log_Debug);
		mono_add_internal_call("Engine.InternalCalls::Utility_Log_Error", Utility_Log_Error);
		mono_add_internal_call("Engine.InternalCalls::Utility_Log_Info", Utility_Log_Info);
		mono_add_internal_call("Engine.InternalCalls::Utility_Log_Warn", Utility_Log_Warn);

		mono_add_internal_call("Engine.InternalCalls::Entity_FindByName", Entity_FindEntityByName);
		mono_add_internal_call("Engine.InternalCalls::Entity_CreateEntityByCopy", Entity_CreateEntityByCopy);


		// Components
		mono_add_internal_call("Engine.InternalCalls::Transform_GetPostition", Transform_GetPostition);
		mono_add_internal_call("Engine.InternalCalls::Transform_SetPostition", Transform_SetPostition);
	}

	

}