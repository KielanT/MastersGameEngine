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

	static std::unordered_map<std::string, ScriptFieldDataTypes> ScriptFieldDataTypeMap =
	{
		{"System.Single", ScriptFieldDataTypes::Float},
		{"System.Int32", ScriptFieldDataTypes::Int32},
		{"System.String", ScriptFieldDataTypes::String},
	};

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

		if (!entity.HasComponent<ScriptComponent>()) return;
		
		auto& ScriptComp = entity.GetComponent<ScriptComponent>();
		if (!m_ScriptInstances.empty())
		{
			if (m_ScriptInstances.count(IDComp.ID) == 0)
			{
				Scripting::GetInstance()->CreateScriptInstance(ScriptComp);
			}

			std::shared_ptr<ScriptInstance> Instance = m_ScriptInstances.find(IDComp.ID)->second;
			if (Instance == nullptr)
			{
				Scripting::GetInstance()->CreateScriptInstance(ScriptComp);
			}

			Instance->OnBegin();
		}
	}

	void Scripting::OnUpdateEntity(Entity entity, float deltaTime)
	{
		const auto& IDComp = entity.GetComponent<IDComponent>();
		const auto& ScriptComp = entity.GetComponent<ScriptComponent>();

		if (!m_ScriptInstances.empty()) 
		{
			if (m_ScriptInstances.count(IDComp.ID) == 0) return;

			std::shared_ptr<ScriptInstance> Instance = m_ScriptInstances.find(IDComp.ID)->second;
			if (Instance != nullptr)
			{
				Instance->OnUpdate(deltaTime);
			}
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

	std::vector<std::string> Scripting::GetAllClassNames()
	{
		// TODO: find a better way this is bad
		
		std::vector<std::string> outVector;
		
		for (const auto klass : m_ClassMaps) 
		{
			outVector.push_back(klass.second->GetMonoClassName());
		}

		return outVector;
	}

	void Scripting::CreateScriptInstance(ScriptComponent& comp)
	{
		if (CheckClassExists(comp.ClassName) && !m_ClassMaps.empty())
		{
			// Create Instance
			std::shared_ptr<ScriptInstance> Instance = std::make_shared<ScriptInstance>(m_ClassMaps.find(comp.ClassName)->second);

			// Add instance to a map using the entity handle
			m_ScriptInstances.insert(std::make_pair(comp.OwnerEntityId, Instance));

		}
	}




	std::shared_ptr<ScriptInstance> Scripting::GetScriptInstance(UUID id)
	{
		if (auto instance = m_ScriptInstances.find(id); instance != m_ScriptInstances.end())
		{
			return instance->second;
		}
		else
		{
			return nullptr;
		}
	}

	std::shared_ptr<ScriptClass> Scripting::GetScriptClassByName(std::string name)
	{
		if (auto instance = m_ClassMaps.find(name); instance != m_ClassMaps.end())
		{
			return instance->second;
		}
		else
		{
			return nullptr;
		}
	}

	void Scripting::SetScene(std::shared_ptr<Scene> scene)
	{
		m_Scene = scene;
	}

	std::shared_ptr<Scene> Scripting::GetScene()
	{
		return m_Scene;
	}

	void Scripting::ResetScripting()
	{
		m_ScriptInstances.clear();
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

			if ((std::string)nameSpace == "Game")
			{
				m_ClassMaps.insert(std::make_pair(name, SC));
			}

			int fieldCount = mono_class_num_fields(SC->GetClass());
			void* it = nullptr;
			while (MonoClassField* field = mono_class_get_fields(SC->GetClass(), &it))
			{
				const char* fieldName = mono_field_get_name(field);

				MonoType* type = mono_field_get_type(field);
				std::string typeName = mono_type_get_name(type);
				
				ScriptFieldDataTypes FieldDataType = ScriptFieldDataTypes::None;
				if (auto search = ScriptFieldDataTypeMap.find(typeName); search != ScriptFieldDataTypeMap.end())
				{
					FieldDataType = search->second;
				}
				
				SC->FieldMap[fieldName] = { FieldDataType, fieldName, field };
			}
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