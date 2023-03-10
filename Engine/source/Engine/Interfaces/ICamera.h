#pragma once

#include "glm/gtc/constants.hpp"

namespace Engine 
{
	enum class ECameraType
	{
		Game = 0,
		Editor
	};

	struct CameraSettings 
	{
		float FOV;
		float Ratio;
		float NearClip;
		float FarClip;

		CameraSettings(float fov = glm::pi<float>() / 3, float aspectRatio = 16.0f / 9.0f, float nearClip = 0.1f, float farClip = 10000.0f) :
			FOV(fov), Ratio(aspectRatio), NearClip(nearClip), FarClip(farClip) 
		{
		}
	};

	class ICamera
	{
		public:
			virtual void SetPosition(glm::vec3 pos) = 0;
			virtual void SetRotation(glm::vec3 rot) = 0;
			virtual void SetCameraSettings(CameraSettings settings) = 0;
			
			virtual glm::vec3 GetPosition() = 0;
			virtual glm::vec3 GetRotation() = 0;
			virtual CameraSettings GetCameraSettings() = 0;

			virtual glm::mat4 GetViewMatrix() = 0;
			virtual glm::mat4 GetEngineionMatrix() = 0;
			virtual glm::mat4 GetViewEngineionMatrix() = 0;
	};

	std::shared_ptr<ICamera> CreateCamera(const ECameraType type);
}

