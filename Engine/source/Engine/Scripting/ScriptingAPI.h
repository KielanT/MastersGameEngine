#pragma once

// Include for allowing foward declarations for mono
#include "mono/utils/mono-forward.h"

namespace Engine
{
	class ScriptingAPI
	{
	public:
		ScriptingAPI() = default;
		~ScriptingAPI() = default;

		ScriptingAPI(const ScriptingAPI&) = delete;
		ScriptingAPI(ScriptingAPI&&) = delete;
		ScriptingAPI& operator=(const ScriptingAPI&) = delete;
		ScriptingAPI& operator=(ScriptingAPI&&) = delete;

	public:
		bool InitScripting();

		void ShutdownScripting();

	private:
		MonoDomain* m_Domain;
	};
}

