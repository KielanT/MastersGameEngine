#pragma once
// Created a class to instantiate the script at the beginning of the program
// Rather than when creating the script instance
// This is done for performance reasons



#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"


namespace Engine
{
	class ScriptClass
	{
	public:
		ScriptClass(const std::string& classNamespace, const std::string& className);

		void OnBegin();

		void InitClassInstance();

	private:
		void SetClass();
		

	private:
		std::string m_ClassNamespace;
		std::string m_ClassName;
		
		MonoClass* m_Class;
		MonoObject* m_ClassInstance;

		MonoMethod* m_OnBegin;
	};
}