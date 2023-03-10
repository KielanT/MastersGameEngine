#pragma once
#include "Engine/Interfaces/ICamera.h"

namespace Engine 
{
	class GameCamera : ICamera
	{
	public:
		virtual void SetPosition(glm::vec3 pos);
		virtual void SetRotation(glm::vec3 rot);
		virtual void SetCameraSettings(CameraSettings settings);

		virtual glm::vec3 GetPosition();
		virtual glm::vec3 GetRotation();
		virtual CameraSettings GetCameraSettings();

		virtual glm::mat4 GetViewMatrix();
		virtual glm::mat4 GetEngineionMatrix();
		virtual glm::mat4 GetViewEngineionMatrix();

	private:
		CameraSettings m_Settings;

		glm::vec3 m_Position;
		glm::vec3 m_Rotation;

		glm::mat4 mWorldMatrix; 
		glm::mat4 mViewMatrix;  

		glm::mat4 mEngineionMatrix;     
		glm::mat4 mViewEngineionMatrix; 
	};
}

