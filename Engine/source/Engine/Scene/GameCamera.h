#pragma once
//**************************************************************************************
// Camera code is from Introduction to 3D Game Programming with DirectX 12 by Frank Luna
//**************************************************************************************

#include "Engine/Interfaces/ICamera.h"

namespace Engine
{

	class GameCamera : public ICamera
	{
	public:
		GameCamera();
		~GameCamera();

		// Setters and getters
		virtual glm::vec3 GetPosition() override;
		virtual void SetPosition(const glm::vec3& pos) override;

		// Get Camera Vectors
		glm::vec3 GetRight()const;
		glm::vec3 GetUp()const;
		glm::vec3 GetLook()const;

		// Get frustum properties.
		float GetNearZ()const;
		float GetFarZ()const;
		float GetAspect()const;
		float GetFovY()const;
		float GetFovX()const;

		// Get near and far plane dimensions in view space coordinates.
		float GetNearWindowWidth()const;
		float GetNearWindowHeight()const;
		float GetFarWindowWidth()const;
		float GetFarWindowHeight()const;


		// Set frustum.
		virtual void SetLens(float fovY, float aspect, float zn, float zf) override;

		// Define camera space via LookAt parameters.
		//void LookAt(DirectX::FXMVECTOR pos, DirectX::FXMVECTOR target, DirectX::FXMVECTOR worldUp);
		//void LookAt(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& target, const DirectX::XMFLOAT3& up);

		// Get View/Proj matrices.
		virtual glm::mat4 GetViewMatrix() override;
		virtual glm::mat4 GetProjectionMatrix() override;
		virtual glm::mat4 GetViewProjectionMatrix() override;

		// Strafe/Walk the camera a distance d.
		void Strafe(float d);
		void Walk(float d);

		// Rotate the camera.
		void Pitch(float angle);
		void RotateY(float angle);


		// After modifying camera position/orientation, call to rebuild the view matrix.
		virtual void UpdateViewMatrix() override;

		virtual void Control(float frameTime) override;

	private:
		// Camera coordinate system with coordinates relative to world space.
		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_Right = { 1.0f, 0.0f, 0.0f };
		glm::vec3 m_Up = { 0.0f, 1.0f, 0.0f };
		glm::vec3 m_Look = { 0.0f, 0.0f, 1.0f };

		// Cache frustum properties.
		float m_NearZ = 0.0f;
		float m_FarZ = 0.0f;
		float m_Aspect = 0.0f;
		float m_FovY = 0.0f;
		float m_NearWindowHeight = 0.0f;
		float m_FarWindowHeight = 0.0f;

		bool m_ViewDirty = true;

		// Cache View/Proj matrices.
		glm::mat4 m_View = glm::mat4();
		glm::mat4 m_Proj = glm::mat4();
	};

}

