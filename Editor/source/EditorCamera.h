#pragma once

#include <Engine.h>

namespace Engine
{
    class EditorCamera : public ICamera
    {
	public:
		EditorCamera() = default;
		
		virtual glm::vec3 GetPosition();
		virtual void SetPosition(const glm::vec3& pos);

		virtual void SetLens(float fovY, float aspect, float zn, float zf);

		virtual glm::mat4 GetViewMatrix();
		virtual glm::mat4 GetProjectionMatrix();
		virtual glm::mat4 GetViewProjectionMatrix();

		virtual void UpdateViewMatrix();

		virtual void Control(float frameTime);

	private:
		// Strafe/Walk the camera a distance d.
		void Strafe(float d);
		void Walk(float d);

		// Rotate the camera.
		void Pitch(float angle);
		void RotateY(float angle);

	private:
		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_Right = { 1.0f, 0.0f, 0.0f };
		glm::vec3 m_Up = { 0.0f, 1.0f, 0.0f };
		glm::vec3 m_Look = { 0.0f, 0.0f, 1.0f };

		glm::mat4 m_View = glm::mat4();
		glm::mat4 m_Proj = glm::mat4();

		// Cache frustum properties.
		float m_NearZ = 0.0f;
		float m_FarZ = 0.0f;
		float m_Aspect = 0.0f;
		float m_FovY = 0.0f;
		float m_NearWindowHeight = 0.0f;
		float m_FarWindowHeight = 0.0f;

		bool m_ViewDirty = true;

    };

}