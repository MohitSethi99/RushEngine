#include "rspch.h"
#include "PerspectiveCamera.h"

#include <glm/gtc/matrix_transform.hpp>
#include "Rush/KeyCodes.h"

namespace Rush
{
	PerspectiveCamera::PerspectiveCamera(float fov, float width, float height, float nearPlane, float farPlane)
		: m_ProjectionMatrix(glm::perspective(glm::radians(fov), (float)width/(float)height, nearPlane, farPlane)), m_ViewMatrix(1.0f),
		m_FOV(fov), m_Width(width), m_Height(height), m_NearPlane(nearPlane), m_FarPlane(farPlane)
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

	void PerspectiveCamera::RecalculateProjectionMatrix()
	{
		m_ProjectionMatrix = glm::perspective(glm::radians(m_FOV), (float)m_Width / (float)m_Height, m_NearPlane, m_FarPlane);
		RecalculateViewMatrix();
	}

	void PerspectiveCamera::Move(CameraMoveDirection direction, float deltaTime)
	{
		float velocity = m_TravelSpeed * deltaTime;

		if (direction == FORWARD)
			m_Position += m_Front * velocity;
		else if (direction == BACK)
			m_Position -= m_Front * velocity;

		if (direction == RIGHT)
			m_Position += m_Right * velocity;
		else if (direction == LEFT)
			m_Position -= m_Right * velocity;

		if (direction == UP)
			m_Position += m_Up * velocity;
		else if (direction == DOWN)
			m_Position -= m_Up * velocity;

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
		if (m_FOV >= 1.0f && m_FOV <= 100)
			m_FOV -= yoffset;
		if (m_FOV <= 1.0f)
			m_FOV = 1.0f;
		if (m_FOV >= 100)
			m_FOV = 100;

		RecalculateProjectionMatrix();
	}
}