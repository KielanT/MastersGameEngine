#include "epch.h"
#include "ScriptingCalls.h"

#include "mono/metadata/object.h"

namespace Engine
{
	static void Utility_Log_Info(MonoString* message)
	{
		MonoError error;
		std::string messageStr(mono_string_to_utf8_checked(message, &error));
		LOG_INFO(messageStr);
	}



	void ScriptingCalls::RegisterScriptFunctions()
	{
		mono_add_internal_call("Engine.InternalCalls::Utility_Log_Info", Utility_Log_Info);
	}

	

}