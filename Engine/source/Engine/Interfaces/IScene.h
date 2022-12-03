#pragma once

#include "Engine/Lab/Camera.h"
#include "entt/entt.hpp"
#include "Engine/Interfaces/IRenderer.h"

namespace Engine
{

	class IScene
	{
	public:
		//virtual ~IScene() = 0;

		// Initializes the geometry for the scene
		virtual bool InitGeometry() = 0;

		// Initializes the scene
		virtual bool InitScene() = 0;

		// Renders the scene
		virtual void RenderScene() = 0;

		// Updates the scene
		virtual void UpdateScene(float frameTime) = 0;

		// Releases the resource of the scene
		virtual void ReleaseResources() = 0;

		// Returns the scene index
		virtual int GetSceneIndex() = 0;

		// Returns the camera
		virtual std::shared_ptr<Camera> GetCamera() = 0;

		virtual std::shared_ptr<IRenderer> GetRenderer() = 0;

		// Scene Settings
		virtual void SetAmbientColour(glm::vec3 ambientColour) = 0;
		virtual void SetSpecularPower(float specularPower) = 0;
		virtual void SetBackgroundColour(glm::vec4 backgroundColour) = 0;
		virtual void SetVSync(bool VSync) = 0;

		virtual glm::vec3 GetAmbientColour() = 0;
		virtual float GetSpecularPower() = 0;
		virtual glm::vec4 GetBackgroundColour() = 0;
		virtual bool GetVSync() = 0;

		virtual entt::registry& GetEntityRegistry() = 0;
	};
}