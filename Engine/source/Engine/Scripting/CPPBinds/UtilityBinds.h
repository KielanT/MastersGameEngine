#pragma once

#include "mono/metadata/object.h"

static void Utility_Log_Debug(MonoString* message)
{
	MonoError error;
	std::string messageStr(mono_string_to_utf8_checked(message, &error));
	LOG_DEBUG(messageStr);
}

static void Utility_Log_Error(MonoString* message)
{
	MonoError error;
	std::string messageStr(mono_string_to_utf8_checked(message, &error));
	LOG_ERROR(messageStr);
}



static void Utility_Log_Info(MonoString* message)
{
	MonoError error;
	std::string messageStr(mono_string_to_utf8_checked(message, &error));
	LOG_INFO(messageStr);
}

static void Utility_Log_Warn(MonoString* message)
{
	MonoError error;
	std::string messageStr(mono_string_to_utf8_checked(message, &error));
	LOG_WARN(messageStr);
}
