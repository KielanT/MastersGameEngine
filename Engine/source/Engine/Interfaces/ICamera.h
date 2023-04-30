#pragma once


namespace Engine
{

	class ICamera
	{
	public: 
		virtual glm::vec3 GetPosition() = 0;
		virtual void SetPosition(const glm::vec3& pos) = 0;

		virtual void SetLens(float fovY, float aspect, float zn, float zf) = 0;

		virtual glm::mat4 GetViewMatrix() = 0;
		virtual glm::mat4 GetProjectionMatrix() = 0;
		virtual glm::mat4 GetViewProjectionMatrix() = 0;
		
		virtual void UpdateViewMatrix() = 0;

		virtual void Control(float frameTime) = 0;

	};
}
