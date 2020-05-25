#pragma once

#include <glm/glm.hpp>

namespace Rush
{
	class PerspectiveCamera
	{
	public:
		PerspectiveCamera(float fov, float width, float height, float nearPlane, float farPlane);

		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); }

		const glm::vec3& GetRotation() const { return glm::vec3(m_Pitch, m_Yaw, m_Rotation); }

		void Move(int keycode, float deltaTime);
		void Rotate(float xoffset, float yoffset);
		void Zoom(float yoffset);

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

	private:
		void RecalculateViewMatrix();
	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_Position = { 0.0f, 0.0f, -10.0f };
		float m_Rotation = 0.0f;
	private:
		glm::vec3 m_Front = { 0.0f, 0.0f, 5.0f };
		glm::vec3 m_Up = { 0.0f, 1.0f, 0.0f };
		glm::vec3 m_Right = { 1.0f, 0.0f, 0.0f };

		float m_Yaw = 90.0f;
		float m_Pitch = 0.0f;
		
		float m_MovementSpeed = 5.0f;
		float m_MouseSensitivity = 0.1f;
		float m_Zoom = 60.0f;
	};
}