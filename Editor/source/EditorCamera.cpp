#include "EditorCamera.h"
#include "Engine/Platform/SDLInput.h"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtx/transform.hpp>


namespace Engine
{
	glm::vec3 EditorCamera::GetPosition()
	{
		return m_Position;
	}

	void EditorCamera::SetPosition(const glm::vec3& pos)
	{
		m_Position = pos;
		m_ViewDirty = true;
	}

	void EditorCamera::SetLens(float fovY, float aspect, float zn, float zf)
	{
		m_FovY = fovY;
		m_Aspect = aspect;
		m_NearZ = zn;
		m_FarZ = zf;

		m_NearWindowHeight = 2.0f * m_NearZ * tanf(0.5f * m_FovY);
		m_FarWindowHeight = 2.0f * m_FarZ * tanf(0.5f * m_FovY);

		m_Proj = glm::perspective(m_FovY, m_Aspect, m_NearZ, m_FarZ);
	}

	glm::mat4 EditorCamera::GetViewMatrix()
	{
		return m_View;
	}

	glm::mat4 EditorCamera::GetProjectionMatrix()
	{
		return m_Proj;
	}

	glm::mat4 EditorCamera::GetViewProjectionMatrix()
	{
		return m_View * m_Proj;
	}

	void EditorCamera::UpdateViewMatrix()
	{
		if (m_ViewDirty)
		{
			m_Look = glm::normalize(m_Look);
			m_Up = glm::normalize(glm::cross(m_Look, m_Right));

			m_Right = glm::cross(m_Up, m_Look);

			// Fill in the view matrix entries.
			float x = -glm::dot(m_Position, m_Right);
			float y = -glm::dot(m_Position, m_Up);
			float z = -glm::dot(m_Position, m_Look);


			m_View[0][0] = m_Right.x;
			m_View[1][0] = m_Right.y;
			m_View[2][0] = m_Right.z;
			m_View[3][0] = x;

			m_View[0][1] = m_Up.x;
			m_View[1][1] = m_Up.y;
			m_View[2][1] = m_Up.z;
			m_View[3][1] = y;

			m_View[0][2] = m_Look.x;
			m_View[1][2] = m_Look.y;
			m_View[2][2] = m_Look.z;
			m_View[3][2] = z;

			m_View[0][3] = 0.0f;
			m_View[1][3] = 0.0f;
			m_View[2][3] = 0.0f;
			m_View[3][3] = 1.0f;

			m_ViewDirty = false;
		}
	}

	void EditorCamera::Control(float frameTime)
	{
		if (SDLInput::KeyHeld(SDLK_DOWN)/*KeyHeld(Key_Down)*/)
		{
			Pitch(glm::radians(60.0f) * frameTime);
			//mRotation.x += ROTATION_SPEED * frameTime; // Use of frameTime to ensure same speed on different machines
		}
		else if (SDLInput::KeyHeld(SDLK_UP)/*KeyHeld(Key_Up)*/)
		{
			Pitch(glm::radians(-60.0f) * frameTime);
			//mRotation.x -= ROTATION_SPEED * frameTime;
		}
		else if (SDLInput::KeyHeld(SDLK_RIGHT)/*KeyHeld(Key_Right)*/)
		{
			RotateY(glm::radians(60.0f) * frameTime);
		}
		else if (SDLInput::KeyHeld(SDLK_LEFT)/*KeyHeld(Key_Left)*/)
		{
			RotateY(glm::radians(-60.0f) * frameTime);
			//mRotation.y -= ROTATION_SPEED * frameTime;
		}
		
		//**** LOCAL MOVEMENT ****
		if (SDLInput::KeyHeld(SDLK_d)/*KeyHeld(Key_D)*/)
		{
			Strafe(50.0f * frameTime);
		}
		if (SDLInput::KeyHeld(SDLK_a)/*KeyHeld(Key_A)*/)
		{
			Strafe(-50.0f * frameTime);
		}
		if (SDLInput::KeyHeld(SDLK_w)/*KeyHeld(Key_W)*/)
		{
			Walk(-50.0f * frameTime);
		}
		if (SDLInput::KeyHeld(SDLK_s)/*KeyHeld(Key_S)*/)
		{
			Walk(50.0f * frameTime);
		}

		UpdateViewMatrix();
	}

	void EditorCamera::Strafe(float d)
	{
		m_Position += d * m_Right;

		m_ViewDirty = true;
	}

	void EditorCamera::Walk(float d)
	{
		m_Position += d * m_Look;

		m_ViewDirty = true;
	}

	void EditorCamera::Pitch(float angle)
	{
		glm::mat4 r = glm::rotate(angle, m_Right);

		/////glm::vec4 up4(m_Up, 0.0f);
		/////m_Up = up4 * r;
		///
		glm::vec4 look4(m_Look, 0.0f); // Check this
		m_Look = look4 * r;
		//
		m_ViewDirty = true;
	}

	void EditorCamera::RotateY(float angle)
	{
		glm::mat4 r = glm::rotate(angle, m_Up);

		//glm::vec4 right4(m_Right, 0.0f);
		//m_Right = right4 * r;

		//glm::vec4 up4(m_Up, 0.0f);
		//m_Up = up4 * r;
		//
		glm::vec4 look4(m_Look, 0.0f); // Check this
		m_Look = look4 * r;

		m_ViewDirty = true;
	}


}