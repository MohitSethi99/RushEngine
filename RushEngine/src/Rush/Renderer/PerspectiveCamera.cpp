#include "rspch.h"
#include "PerspectiveCamera.h"

#include <glm/gtc/matrix_transform.hpp>
#include "Rush/KeyCodes.h"

namespace Rush
{
	PerspectiveCamera::PerspectiveCamera(float fov, float width, float height, float nearPlane, float farPlane)
		: m_ProjectionMatrix(glm::perspective(glm::radians(fov), (float)width/(float)height, nearPlane, farPlane)), m_ViewMatrix(1.0f)
	{
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
		RecalculateViewMatrix();
	}

	void PerspectiveCamera::RecalculateViewMatrix()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		front.y = sin(glm::radians(m_Pitch));
		front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));

		m_Front = glm::normalize(front);
		m_Right = glm::normalize(glm::cross(m_Front, { 0.0f, 1.0f, 0.0f }));
		m_Up = glm::normalize(glm::cross(m_Right, m_Front));

		m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void PerspectiveCamera::Move(int keycode, float deltaTime)
	{
		float velocity = 5 * deltaTime;
		if (keycode == RS_KEY_W)
			m_Position += m_Front * velocity;
		if (keycode == RS_KEY_S)
			m_Position -= m_Front * velocity;
		if (keycode == RS_KEY_A)
			m_Position -= m_Right * velocity;
		if (keycode == RS_KEY_D)
			m_Position += m_Right * velocity;

		RecalculateViewMatrix();
	}

	void PerspectiveCamera::Rotate(float xoffset, float yoffset)
	{
		xoffset *= m_MouseSensitivity;
		yoffset *= m_MouseSensitivity;

		m_Yaw += xoffset;
		m_Pitch += yoffset;

		if (m_Pitch > 89.0f)
			m_Pitch = 89.0f;
		if (m_Pitch < -89.0f)
			m_Pitch = -89.0f;

		RecalculateViewMatrix();
	}

	void PerspectiveCamera::Zoom(float yoffset)
	{
		if (m_Zoom >= 1.0f && m_Zoom <= 100)
			m_Zoom -= yoffset;
		if (m_Zoom <= 1.0f)
			m_Zoom = 1.0f;
		if (m_Zoom >= 100)
			m_Zoom = 100;

		RecalculateViewMatrix();
	}
}