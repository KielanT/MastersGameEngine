#pragma once

#include "Engine/Lab/Camera.h"
#include "Engine/Renderer/DirectX11/DirectX11Renderer.h"
#include "Engine/Interfaces/IShader.h"
#include "Engine/Interfaces/IState.h"

#include "entt/entt.hpp"

namespace Engine
{
	class Entity;

	struct SceneSettings
	{
		glm::vec3 ambientColour = glm::vec3(0.2f, 0.2f, 0.3f);
		float specularPower = 256.0f;
		glm::vec4 backgroundColour = glm::vec4(0.2f, 0.2f, 0.3f, 1.0f);
		bool vsyncOn = true;
	};

	class Scene : public std::enable_shared_from_this<Scene>
	{
	public:
		Scene(std::shared_ptr<IRenderer> renderer);
		~Scene();

		void InitScene();
		void RenderScene();
		void UpdateScene(float frametime);
		void RemoveScene();

		entt::registry& GetEntityRegistry() { return m_Registry; }
		Entity CreateEntity(const std::string& tag);
		Entity CreateMeshEntity(const std::string& tag);
		void DeleteEntity(Entity entity);

		SceneSettings GetSceneSettings() { return m_SceneSettings; }
		std::shared_ptr<Camera> GetCamera() { return m_MainCamera; }

	private:
		SceneSettings m_SceneSettings;
		std::shared_ptr<Camera> m_MainCamera = nullptr;
		entt::registry m_Registry;

	public: // TODO: Remove all renderer specific code
		std::shared_ptr<IRenderer> GetRenderer() { return m_Renderer; }

	private: // TODO: Remove all renderer specific code
		void Renderer(Entity entity);
		std::shared_ptr<IRenderer> m_Renderer;

		std::shared_ptr<IShader> m_Shader;
		std::shared_ptr<IState> m_State;
	};

}
