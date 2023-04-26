#pragma once


#include "Engine/Lab/Camera.h"
#include "Engine/Interfaces/ICamera.h"
#include "Engine/UUID.h"
#include "entt/entt.hpp"
#include <filesystem>

namespace Engine
{
	class Entity;

	

	struct SceneSettings
	{
		std::string title = "untitled";
		eint32 index = 0;
		glm::vec3 ambientColour = glm::vec3(0.2f, 0.2f, 0.3f);
		float specularPower = 256.0f;
		glm::vec4 backgroundColour = glm::vec4(0.2f, 0.2f, 0.3f, 1.0f);
		bool vsyncOn = true;
		std::filesystem::path assetFilePath = "";
	};


	class Scene : public std::enable_shared_from_this<Scene>
	{
	public:
		Scene() {};
		~Scene();

		void InitScene();
		void UnloadScene();
		void RenderScene();

		void BeginScene();
		void UpdateScene(float frametime);
		void RemoveScene();

		void SimulateScene(float frametime);
		void EditorUpdatePhysicsScene(float frametime);

		//void SetSkybox(std::string meshPath, std::string texPath, std::string assetPath);
		entt::registry& GetEntityRegistry() { return m_Registry; }
		Entity CreateEntity(const std::string& tag);
		Entity CreateEntityWithUUID(UUID uuid, const std::string& tag);
		Entity CreateMeshEntity(const std::string& tag);
		Entity CreateSkyboxEntity(const std::string& tag);
		Entity CreateCameraEntity(const std::string& tag);
		void DeleteEntity(Entity entity);
		void LoadEntities(std::string assetPath);
		void SetActiveCamera();

		SceneSettings GetSceneSettings() { return m_SceneSettings; }
		void SetSceneSettings(SceneSettings& settings) { m_SceneSettings = settings; }
		//std::shared_ptr<Camera> GetCamera() { return m_MainCamera; }
		std::shared_ptr<ICamera> GetCamera() { return m_MainCamera; }
		void SetCamera(std::shared_ptr<ICamera> Cam) { m_MainCamera = Cam; }

		SceneSettings m_SceneSettings;

		template<typename T>
		void OnComponentCreated(Entity entity, T& comp);

		Entity FindEntityByName(const std::string& name);
		Entity FindEntityByUUID(UUID id);

	private:
		void LoadEntity(Entity entity, std::string& assetPath);

	
	private:
		
		std::shared_ptr<ICamera> m_MainCamera = nullptr;


		entt::registry m_Registry;

	};

	

}
