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
		bool InitScripting();
		void ShutdownScripting();

		void OnBeginEntity(Entity entity);
		void OnUpdateEntity(Entity entity, float deltaTime);

		_MonoAssembly* GetAssembly();
		MonoDomain* GetAppDomain();

		std::vector<std::string> GetAllClassNames();
		void CreateScriptInstance(ScriptComponent& comp);
		std::shared_ptr<ScriptInstance> GetScriptInstance(UUID id);
		
		std::shared_ptr<ScriptClass> GetScriptClassByName(std::string name);

		void SetScene(std::shared_ptr<Scene> scene);
		std::shared_ptr<Scene> GetScene();



	private:
		bool InitMono();

		_MonoAssembly* LoadAssembly(const std::string& assemblyPath);

		void LoadAssemblyClass();

		// Helper function
		char* ReadBytes(const std::string& filepath, uint32_t* outSize);

		bool CheckClassExists(const std::string& className);

	private:
		static std::shared_ptr<Scripting> m_Scripting;

	private:
		MonoDomain* m_Domain;
		MonoDomain* m_AppDomain;
		_MonoAssembly* m_Assembly;

		// std::sting = class name	// ScriptClassData contains class data
		std::unordered_map<std::string, std::shared_ptr<ScriptClass>> m_ClassMaps;

		std::unordered_map<UUID, std::shared_ptr<ScriptInstance>> m_ScriptInstances;

		std::shared_ptr<Scene> m_Scene;
	};

}

