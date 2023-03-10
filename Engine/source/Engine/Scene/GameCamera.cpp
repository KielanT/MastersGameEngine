#include "epch.h"
#include "GameCamera.h"

namespace Engine
{
	void GameCamera::SetPosition(glm::vec3 pos)
	{
	}

	void GameCamera::SetRotation(glm::vec3 rot)
	{
	}

	void GameCamera::SetCameraSettings(CameraSettings settings)
	{
	}

	glm::vec3 GameCamera::GetPosition()
	{
		return m_Position;
	}

	glm::vec3 GameCamera::GetRotation()
	{
		return m_Position;
	}

	CameraSettings GameCamera::GetCameraSettings()
	{
		return m_Settings;
	}

	glm::mat4 GameCamera::GetViewMatrix()
	{
		return glm::mat4();
	}

	glm::mat4 GameCamera::GetEngineionMatrix()
	{
		return glm::mat4();
	}

	glm::mat4 GameCamera::GetViewEngineionMatrix()
	{
		return glm::mat4();
	}

}
