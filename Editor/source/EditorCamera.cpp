#include "EditorCamera.h"

namespace Engine
{

	void EditorCamera::SetPosition(glm::vec3 pos)
	{
		m_Position = pos;
	}

	void EditorCamera::SetRotation(glm::vec3 rot)
	{
		m_Rotation = rot;
	}

	void EditorCamera::SetCameraSettings(CameraSettings settings)
	{
		m_Settings = settings;
	}

	glm::vec3 EditorCamera::GetPosition()
	{
		return m_Position;
	}

	glm::vec3 EditorCamera::GetRotation()
	{
		return m_Rotation;
	}

	CameraSettings EditorCamera::GetCameraSettings()
	{
		return m_Settings;
	}

	glm::mat4 EditorCamera::GetViewMatrix()
	{
		UpdateMatrices();
		return m_ViewMatrix;
	}

	glm::mat4 EditorCamera::GetEngineionMatrix()
	{
		UpdateMatrices();
		return m_EngineionMatrix;
	}

	glm::mat4 EditorCamera::GetViewEngineionMatrix()
	{
		UpdateMatrices();
		return m_ViewEngineionMatrix;
	}

	void EditorCamera::UpdateMatrices()
	{

		glm::mat4 rotationZ = temp::MatRotZ(m_Rotation.z);
		glm::mat4 rotationX = temp::MatRotX(m_Rotation.x);
		glm::mat4 rotationY = temp::MatRotY(m_Rotation.y);
		glm::mat4 transMat = temp::MatTranslation(m_Position);

		// Solution 1
		glm::mat4 ZX = temp::MatrixMultiplication(rotationZ, rotationX);
		glm::mat4 ZXY = temp::MatrixMultiplication(ZX, rotationY);
		glm::mat4 ZXYT = temp::MatrixMultiplication(ZXY, transMat);
		m_WorldMatrix = ZXYT;

		// Solution 2
		//mWorldMatrix = MatrixMultiplication(MatrixMultiplication(rotationZ, rotationX), MatrixMultiplication(rotationY, transMat));

		// View matrix is the usual matrix used for the camera in shaders, it is the inverse of the world matrix (see lectures)
		m_ViewMatrix = glm::inverse(m_WorldMatrix);// InverseAffine(mat);
		//mViewMatrix = InverseAffine(mWorldMatrix);

		// Engineion matrix, how to flatten the 3D world onto the screen (needs field of view, near and far clip, aspect ratio)
		float tanFOVx = std::tan(m_Settings.FOV * 0.5f);
		float scaleX = 1.0f / tanFOVx;
		float scaleY = m_Settings.Ratio / tanFOVx;
		float scaleZa = m_Settings.FarClip / (m_Settings.FarClip - m_Settings.NearClip);
		float scaleZb = -m_Settings.NearClip * scaleZa;

		m_EngineionMatrix = { scaleX,   0.0f,    0.0f,   0.0f,
								0.0f, scaleY,    0.0f,   0.0f,
								0.0f,   0.0f, scaleZa,   1.0f,
								0.0f,   0.0f, scaleZb,   0.0f };

		// The view-Engineion matrix combines the two matrices usually used for the camera into one, which can save a multiply in the shaders (optional)
		m_ViewEngineionMatrix = m_ViewMatrix * m_EngineionMatrix;
	}

}