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

	void ScriptClass::OnUpdate(float deltaTime)
	{
		if (m_OnUpdate != nullptr && m_ClassInstance != nullptr)
		{
			MonoObject* exception = nullptr;

			// mono_runtime_invoke slow but does a lot of checks
			// mono_method_get_unmanaged_thunk is faster since it has less overhead (eg best for update functions) (not sure how to use)
			void* param = &deltaTime;
			mono_runtime_invoke(m_OnUpdate, m_ClassInstance, &param, &exception);
		

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
			m_OnUpdate = mono_class_get_method_from_name(m_Class, "OnUpdate", 1);

		}
	}

	std::string ScriptClass::GetMonoClassName()
	{
		return m_ClassName;
	}

}