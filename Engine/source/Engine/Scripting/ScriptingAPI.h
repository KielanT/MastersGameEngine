#pragma once

//*************************************************
// The word assembly is used to refer to the DLL
// 
// Documentations used:
//  - https://nilssondev.com/mono-guide/book/introduction.html
//  - https://www.mono-project.com/docs/advanced/embedding/
//*************************************************

// Include for allowing foward declarations for mono
#include "mono/utils/mono-forward.h"


class _MonoAssembly;
class _MonoClass;
class _MonoObject;

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

		// Test Vars
		void CallTestScript();

	private:
		bool InitMono();

		_MonoAssembly* LoadAssembly(const std::string& assemblyPath);

		// Used for checking that all the types in the assemblies
		// Metadata works
		void PrintAssemblyTypes(_MonoAssembly* assembly);

		// Helper function
		char* ReadBytes(const std::string& filepath, uint32_t* outSize);

	private:
		// ****************** 
		// Testing functions
		// ****************** 

		void MainTestFunction();

		// Get class
		_MonoClass* GetClassInAssembly(const char* nameSpace, const char* name);

		// Call specific method from c#
		void CallMethod(_MonoObject* objectInstance);

		void CallIncrementMethod(_MonoObject* objectInstance, float value);

		void PropertiesAndFieldsTest(_MonoObject* objectInstance);

		

	private:
		MonoDomain* m_Domain;
		MonoDomain* m_AppDomain;
		_MonoAssembly* m_Assembly;
	};
}

