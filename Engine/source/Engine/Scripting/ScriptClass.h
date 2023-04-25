#pragma once
// Created a class to instantiate the script at the beginning of the program
// Rather than when creating the script instance
// This is done for performance reasons



#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"


namespace Engine
{
	enum class ScriptFieldDataTypes
	{
		None = 0,
		Float, Int32
	};

	struct ScriptField
	{
		ScriptFieldDataTypes FieldDataType;
		std::string ScriptName;

		MonoClassField* ClassField;
	};

	class ScriptClass
	{
	public:
		ScriptClass(const std::string& classNamespace, const std::string& className);

		void OnBegin();
		void OnUpdate(float deltaTime);

		void InitClassInstance();

		std::string GetMonoClassName();
		MonoClass* GetClass();

		template<typename T>
		T GetFieldValue(const std::string& name)
		{
			if (auto search = FieldMap.find(name); search != FieldMap.end())
			{
				
				char s_FieldValueBuffer[16];
				void* value = s_FieldValueBuffer;
				mono_field_get_value(m_ClassInstance, search->second.ClassField, value);
				
				return *(T*)value;
			}
		}

		template<typename T>
		void SetFieldValue(const std::string& name, T value)
		{
			if (auto search = FieldMap.find(name); search != FieldMap.end())
			{

				mono_field_set_value(m_ClassInstance, search->second.ClassField, (void*)&value);

			}
		}

		std::map<std::string, ScriptField> FieldMap;



	private:
		void SetClass();
		

	private:
		std::string m_ClassNamespace;
		std::string m_ClassName;
		
		MonoClass* m_Class;
		MonoObject* m_ClassInstance;

		MonoMethod* m_OnBegin;
		MonoMethod* m_OnUpdate;

		
	};
}