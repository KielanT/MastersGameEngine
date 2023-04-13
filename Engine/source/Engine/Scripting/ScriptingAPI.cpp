#include "epch.h"
#include "ScriptingAPI.h"
#include <winver.h>
#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"

#include <filesystem>

namespace Engine
{

	bool ScriptingAPI::InitScripting()
	{
		
		std::filesystem::path path = std::filesystem::current_path().parent_path();
		path.append("Engine\\external\\mono\\lib\\4.5");
		mono_set_assemblies_path(path.string().c_str());
		

		MonoDomain* domain = mono_jit_init("ScriptRuntime");
		if (domain == nullptr) return false;

		m_Domain = domain;

		return true;
	}

	void ScriptingAPI::ShutdownScripting()
	{
		if (m_Domain != nullptr) delete m_Domain;
	}
}