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

#include "ScriptInstance.h"
#include "ScriptClass.h"

#include "Engine/Scene/Scene.h"
#include "Engine/Scene/Components.h"

class _MonoAssembly;
class _MonoClass;
class _MonoObject;

//**********************************************************
// Scripting is a singleton for use anywhere in the project
//**********************************************************


namespace Engine
{

	class Scripting
	{

	public:
		Scripting() = default;
		~Scripting() = default;

		Scripting(const Scripting&) = delete;
		Scripting(Scripting&&) = delete;
		Scripting& operator=(const Scripting&) = delete;
		Scripting& operator=(Scripting&&) = delete;

		static std::shared_ptr<Scripting> GetInstance();

	public:
		
		// Initalise scripting
		bool InitScripting();
		void ShutdownScripting();

		// Calls the Methods for the entities
		void OnBeginEntity(Entity entity);
		void OnUpdateEntity(Entity entity, float deltaTime);
		void OnContactEntity(Entity entity);

		//*************
		// Getters
		//*************
		_MonoAssembly* GetAssembly();
		MonoDomain* GetAppDomain();

		std::vector<std::string> GetAllClassNames();
		void CreateScriptInstance(ScriptComponent& comp);
		std::shared_ptr<ScriptInstance> GetScriptInstance(UUID id);
		
		std::shared_ptr<ScriptClass> GetScriptClassByName(std::string name);

		// Set the scene for calling the scene where needed
		void SetScene(std::shared_ptr<Scene> scene);
		std::shared_ptr<Scene> GetScene();

		// Reset the scripts
		void ResetScripting();

	private:
		// Set up mono
		bool InitMono();

		// Load the DLL
		_MonoAssembly* LoadAssembly(const std::string& assemblyPath);

		// Load the classes in DLL
		void LoadAssemblyClass();

		// Helper function
		char* ReadBytes(const std::string& filepath, uint32_t* outSize);
		
		// Checks if the class exists
		bool CheckClassExists(const std::string& className);


	private:
		static std::shared_ptr<Scripting> m_Scripting;

	private:
		MonoDomain* m_Domain;
		MonoDomain* m_AppDomain;
		_MonoAssembly* m_Assembly;

		// Maps for storing the classes 
		// This is used for creating unique classes for the instances
		// std::sting = class name	// ScriptClassData contains class data
		std::unordered_map<std::string, std::shared_ptr<ScriptClass>> m_ClassMaps;

		// Instance is the class instance for the entity
		std::unordered_map<UUID, std::shared_ptr<ScriptInstance>> m_ScriptInstances;

		std::shared_ptr<Scene> m_Scene;
	};

}

