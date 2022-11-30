#pragma once
#include "Engine/SceneSystem/DirectX11/DirectX11SceneManager.h"
#include "Engine/Interfaces/IScene.h"
#include "Engine/Interfaces/IRenderer.h"
#include "Engine/Interfaces/IShader.h"
#include "Engine/Interfaces/IState.h"

#include "Engine/Lab/Model.h"
#include "Entity.h"

namespace Engine
{

	class TestScene : public IScene, public std::enable_shared_from_this<TestScene>
	{
	public:
		TestScene(DirectX11SceneManager* sceneManager, std::shared_ptr<IRenderer> renderer, int sceneIndex, glm::vec3 ambientColour = glm::vec3(0.2f, 0.2f, 0.3f),
			float specularPower = 256.0f, glm::vec4 backgroundColour = glm::vec4(0.2f, 0.2f, 0.3f, 1.0f),
			bool vsyncOn = true);

		virtual bool InitGeometry() override;

		virtual bool InitScene() override;

		virtual void RenderScene() override;

		virtual void UpdateScene(float frameTime) override;

		virtual void ReleaseResources() override;

		virtual int GetSceneIndex() override { return m_SceneIndex; }

		virtual std::shared_ptr<Camera> GetCamera() override { return m_SceneCamera; }

		// Scene Settings
		virtual void SetAmbientColour(glm::vec3 ambientColour) override { m_AmbientColour = ambientColour; }
		virtual void SetSpecularPower(float specularPower) override { m_SpecularPower = specularPower; }
		virtual void SetBackgroundColour(glm::vec4 backgroundColour) override { m_backgroundColour = backgroundColour; }
		virtual void SetVSync(bool VSync) override { m_VsyncOn = VSync; }

		virtual glm::vec3 GetAmbientColour() override { return m_AmbientColour; }
		virtual float GetSpecularPower() override { return m_SpecularPower; }
		virtual glm::vec4 GetBackgroundColour() override { return m_backgroundColour; }
		virtual bool GetVSync() override { return m_VsyncOn; }

		virtual entt::registry& GetEntityRegistry() override { return m_Registry; }
	
		/*virtual*/ Entity CreateEntity(const std::string& tag) /*override*/;
		/*virtual*/ Entity CreateMeshEntity(const std::string& tag) /*override*/;

	private:
		std::shared_ptr<IRenderer> m_Renderer;
		int m_SceneIndex;

		std::shared_ptr<Camera> m_SceneCamera = nullptr;;


		glm::vec3 m_AmbientColour;
		float m_SpecularPower;
		glm::vec4 m_backgroundColour;
		bool m_VsyncOn;

		DirectX11SceneManager* m_sceneManager = nullptr;

		std::shared_ptr<IShader> m_Shader;
		std::shared_ptr<IState> m_State;
		std::shared_ptr<Mesh> mesh;
		std::shared_ptr<Model> model;

		CComPtr<ID3D11Resource> resource;
		CComPtr<ID3D11ShaderResourceView> resourceView;

		entt::registry m_Registry;

		Entity TempEntity;
	};
}
