#pragma once
#include <Engine.h>

namespace Engine {

	class EditorCamera : ICamera
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
		void UpdateMatrices();

	private:
		CameraSettings m_Settings;

		glm::vec3 m_Position;
		glm::vec3 m_Rotation;

		glm::mat4 m_WorldMatrix;
		glm::mat4 m_ViewMatrix;

		glm::mat4 m_EngineionMatrix;
		glm::mat4 m_ViewEngineionMatrix;

		float RotationSpeed = 2.0f;
		float MovementSpeed = 50.0f;
	};

}