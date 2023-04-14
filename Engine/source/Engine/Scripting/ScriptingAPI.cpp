#include "epch.h"
#include "ScriptingAPI.h"
#include <winver.h>
#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"

#include <filesystem>
#include <string>

namespace Engine
{

	bool ScriptingAPI::InitScripting()
	{
		if (!InitMono())
		{
			LOG_ERROR("Mono Init failed");
			return false;
		}


		m_Assembly = LoadAssembly("Resources/Scripts/EngineScripting.dll");


		MainTestFunction();

		return true;
	}

	void ScriptingAPI::ShutdownScripting()
	{
		// TODO shutdown properly
		if (m_Domain != nullptr) delete m_Domain;
		if (m_AppDomain != nullptr) delete m_AppDomain;
		if (m_Assembly != nullptr) delete m_Assembly;
	}

	bool ScriptingAPI::InitMono()
	{

		std::filesystem::path path = std::filesystem::current_path().parent_path();
		path.append("Engine\\external\\mono\\lib\\4.5");
		mono_set_assemblies_path(path.string().c_str());

		MonoDomain* domain = mono_jit_init("ScriptRuntime");
		if (domain == nullptr) return false;

		m_Domain = domain;

		m_AppDomain = mono_domain_create_appdomain((char*)"AppDomain", nullptr);
		mono_domain_set(m_AppDomain, true);

		return true;
	}

	MonoAssembly* ScriptingAPI::LoadAssembly(const std::string& assemblyPath)
	{
		uint32_t fileSize = 0;
		char* fileData = ReadBytes(assemblyPath, &fileSize);

		MonoImageOpenStatus status;
		MonoImage* image = mono_image_open_from_data_full(fileData, fileSize, 1, &status, 0);
		if (status != MONO_IMAGE_OK)
		{
			const char* errorMessage = mono_image_strerror(status);
			LOG_ERROR(errorMessage);
			return nullptr;
		}

		MonoAssembly* assembly = mono_assembly_load_from_full(image, assemblyPath.c_str(), &status, 0);
		mono_image_close(image);

		delete[] fileData;

		return assembly;
	}

	void ScriptingAPI::PrintAssemblyTypes(_MonoAssembly* assembly)
	{
		MonoImage* image = mono_assembly_get_image(assembly);
		const MonoTableInfo* typeDefinitionTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
		int32_t numTypes = mono_table_info_get_rows(typeDefinitionTable);

		for (int32_t i = 0; i < numTypes; i++)
		{
			uint32_t cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(typeDefinitionTable, i, cols, MONO_TYPEDEF_SIZE);

			const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
			const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

			LOG_INFO("namspace {0}", nameSpace);
			LOG_INFO("name {0}", name);
		}

	}

	char* ScriptingAPI::ReadBytes(const std::string& filepath, uint32_t* outSize)
	{
		std::ifstream stream(filepath, std::ios::binary | std::ios::ate);

		if (!stream)
		{
			LOG_ERROR("Failed to open the file");
			return nullptr;
		}

		std::streampos end = stream.tellg();
		stream.seekg(0, std::ios::beg);
		uint32_t size = end - stream.tellg();

		if (size == 0)
		{
			LOG_WARN("File is empty");
			return nullptr;
		}

		char* buffer = new char[size];
		stream.read((char*)buffer, size);
		stream.close();

		*outSize = size;
		return buffer;
	}

	void ScriptingAPI::MainTestFunction()
	{
		// Get all the types in the assembly 
		PrintAssemblyTypes(m_Assembly);


		// Call class
		MonoClass* testClass = GetClassInAssembly("", "CSharpTesting");
		MonoObject* classInstance = mono_object_new(m_AppDomain, testClass);

		if (classInstance == nullptr)
		{
			LOG_ERROR("Class instance creation failed");
		}

		// Init class with a parameterless constructer
		mono_runtime_object_init(classInstance);

		
		// Call method
		CallMethod(classInstance);
		CallIncrementMethod(classInstance, 5.0f);
		CallMethod(classInstance);

		PropertiesAndFieldsTest(classInstance);

	}

	MonoClass* ScriptingAPI::GetClassInAssembly(const char* nameSpace, const char* name)
	{
		MonoImage* image = mono_assembly_get_image(m_Assembly);
		MonoClass* klass = mono_class_from_name(image, nameSpace, name);

		if (klass == nullptr)
		{
			LOG_ERROR("Didn't find class");
			return nullptr;
		}

		return klass;
	}

	void ScriptingAPI::CallMethod(_MonoObject* objectInstance)
	{
		MonoClass* instanceClass = mono_object_get_class(objectInstance);

		MonoMethod* method = mono_class_get_method_from_name(instanceClass, "PrintVar", 0);

		if (method == nullptr)
		{
			LOG_ERROR("Method not found");
			return;
		}


		// mono_runtime_invoke slow but does a lot of check
		// mono_method_get_unmanaged_thunk is faster since it has less overhead (eg best for update functions)

		MonoObject* exception = nullptr;
		mono_runtime_invoke(method, objectInstance, nullptr, &exception);
		//mono_method_get_unmanaged_thunk(method); // Does not work

		// TODO handle the exception
	}

	void ScriptingAPI::CallIncrementMethod(_MonoObject* objectInstance, float value)
	{
		MonoClass* instanceClass = mono_object_get_class(objectInstance);

		MonoMethod* method = mono_class_get_method_from_name(instanceClass, "IncrementVar", 1);
		if (method == nullptr)
		{
			LOG_ERROR("Method not found");
			return;
		}

		MonoObject* exception = nullptr;
		void* param = &value;
		mono_runtime_invoke(method, objectInstance, &param, &exception);

		// **********************
		// If there are multiple params
		//MonoObject* exception = nullptr;
		//void* params[] = 
		//{
		//	&value
		//};
		//mono_runtime_invoke(method, objectInstance, &params, &exception);

	}

	void ScriptingAPI::PropertiesAndFieldsTest(_MonoObject* objectInstance)
	{
		MonoClass* instanceClass = mono_object_get_class(objectInstance);

		MonoClassField* floatField = mono_class_get_field_from_name(instanceClass, "FloatVar");
		
		MonoClassField* nameField = mono_class_get_field_from_name(instanceClass, "m_Name");
		
		MonoProperty* nameProperty = mono_class_get_property_from_name(instanceClass, "Name");
	
		// Get Float from field
		float floatFromField;
		mono_field_get_value(objectInstance, floatField, &floatFromField);
		LOG_INFO("Float set from c# to c++ {0}", floatFromField);

		// Set float value
		floatFromField += 10.0f;
		mono_field_set_value(objectInstance, floatField, &floatFromField);
		mono_field_get_value(objectInstance, floatField, &floatFromField);
		LOG_INFO("Float set & get from c# to c++ {0}", floatFromField);

		// Get value from a c# getter
		MonoString* nameValue = (MonoString*)mono_property_get_value(nameProperty, objectInstance, nullptr, nullptr);
		MonoError error;
		std::string nameStr(mono_string_to_utf8_checked(nameValue, &error));
		LOG_INFO("string value from c# getter {0}", nameStr);

		// Set value using the c# setter
		nameStr += ", World";
		nameValue = mono_string_new(m_AppDomain, nameStr.c_str());
		mono_property_set_value(nameProperty, objectInstance, (void**)&nameValue, nullptr);
		std::string newNameStr(mono_string_to_utf8_checked(nameValue, &error));
		LOG_INFO("string value from c# setter {0}", newNameStr);
	}


}