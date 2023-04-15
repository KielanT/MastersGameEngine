#include "epch.h"
#include "ScriptClass.h"

#include "Scripting.h"


namespace Engine
{

	ScriptClass::ScriptClass(const std::string& classNamespace, const std::string& className)
		: m_ClassNamespace(classNamespace), m_ClassName(className)
	{
		SetClass();
		//InitClassInstance(); // Causes a crash called inside instance instead
	}

	void ScriptClass::OnBegin()
	{
		if (m_OnBegin != nullptr && m_ClassInstance != nullptr)
		{
			MonoObject* exception = nullptr;
			mono_runtime_invoke(m_OnBegin, m_ClassInstance, nullptr, &exception);

			// TODO: handle exception
		}

	}

	void ScriptClass::SetClass()
	{
		MonoImage* image = mono_assembly_get_image(Scripting::GetInstance()->GetAssembly());
		MonoClass* klass = mono_class_from_name(image, m_ClassNamespace.c_str(), m_ClassName.c_str());

		if (klass == nullptr)
		{
			LOG_ERROR("Class: {0}, NOT FOUND", m_ClassName);
			return;
		}
		
		m_Class = klass;
	}

	void ScriptClass::InitClassInstance()
	{
		if (m_Class != nullptr)
		{
			m_ClassInstance = mono_object_new(Scripting::GetInstance()->GetAppDomain(), m_Class);
			
			if (m_ClassInstance == nullptr)
			{
				LOG_ERROR("Class instance not made");
			}

			// Create instance (Calling parameterless constructer)
			mono_runtime_object_init(m_ClassInstance);

			m_OnBegin = mono_class_get_method_from_name(m_Class, "OnBegin", 0);

		}
	}

}