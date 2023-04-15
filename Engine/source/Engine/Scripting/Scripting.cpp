#include "epch.h"
#include "Scripting.h"
#include "ScriptingCalls.h"
#include "Engine/Scene/Entity.h"

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"

#include <filesystem>
#include <string>


namespace Engine
{
	std::shared_ptr<Scripting> Scripting::m_Scripting = nullptr;

	std::shared_ptr<Scripting> Scripting::GetInstance()
	{
		if (m_Scripting == nullptr)
		{
			m_Scripting = std::make_shared<Scripting>();
		}

		return m_Scripting;
	}
	
	bool Scripting::InitScripting()
	{
		if (!InitMono())
		{
			LOG_ERROR("Mono Init failed");
			return false;
		}

		m_Assembly = LoadAssembly("Resources/Scripts/EngineScripting.dll");
		
		ScriptingCalls::RegisterScriptFunctions();

		LoadAssemblyClass();

		
		
		return true;
	}

	void Scripting::ShutdownScripting()
	{
		// TODO shutdown properly
		if (m_Domain != nullptr) delete m_Domain;
		if (m_AppDomain != nullptr) delete m_AppDomain;
		if (m_Assembly != nullptr) delete m_Assembly;
	}

	void Scripting::OnBeginEntity(Entity entity)
	{
		const auto& IDComp = entity.GetComponent<IDComponent>();
		const auto& ScriptComp = entity.GetComponent<ScriptComponent>();

		if (CheckClassExists(ScriptComp.ClassName))
		{
			// Create Instance
			std::shared_ptr<ScriptInstance> Instance = std::make_shared<ScriptInstance>(m_ClassMaps.find(ScriptComp.ClassName)->second);

			// Add instance to a map using the entity handle

			m_ScriptInstances.insert(std::make_pair(IDComp.ID, Instance));

			// Call script on begin
			Instance->OnBegin();
		}
	}

	_MonoAssembly* Scripting::GetAssembly()
	{
		return m_Assembly;
	}

	MonoDomain* Scripting::GetAppDomain()
	{
		return m_AppDomain;
	}

	bool Scripting::InitMono()
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

	_MonoAssembly* Scripting::LoadAssembly(const std::string& assemblyPath)
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

	void Scripting::LoadAssemblyClass()
	{
		m_ClassMaps.clear();

		MonoImage* image = mono_assembly_get_image(m_Assembly);
		const MonoTableInfo* typeDefinitionTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
		int32_t numTypes = mono_table_info_get_rows(typeDefinitionTable);

		for (int32_t i = 0; i < numTypes; i++)
		{
			uint32_t cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(typeDefinitionTable, i, cols, MONO_TYPEDEF_SIZE);

			const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
			const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

			std::shared_ptr<ScriptClass> SC = std::make_shared<ScriptClass>(nameSpace, name);

			m_ClassMaps.insert(std::make_pair(name, SC));
		}
	}

	char* Scripting::ReadBytes(const std::string& filepath, uint32_t* outSize)
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

	bool Scripting::CheckClassExists(const std::string& className)
	{
		bool DoesExist = m_ClassMaps.find(className) != m_ClassMaps.end();
		return DoesExist;
	}

	
}