#include "pch.h"
#include "camera.h"

namespace ToyEngine
{
	Camera::Camera(const CameraProps& props) : m_data(props)
	{
		UpdateCameraVectors();
	}

	// getter/setter
	glm::mat4 Camera::GetViewMatrix() const
	{
		return glm::lookAt(m_data.position, m_data.position + m_data.front, m_data.up);
	}

	void Camera::SetMovementSpeed(float speed)
	{
		m_data.movementSpeed = speed;
	}
	void Camera::SetMouseSensitivity(float sensitivity)
	{
		m_data.mouseSensitivity = sensitivity;
	}
	void Camera::SetNear(float znear)
	{
		m_data.znear = znear;
	}
	void Camera::SetFar(float zfar)
	{
		m_data.zfar = zfar;
	}

	// call-back
	void Camera::UpdatePosition(eCameraMovement direction, float time_step)
	{
		float velocity = m_data.movementSpeed * time_step;
		switch (direction)
		{
		case eCameraMovement::kForward:
			m_data.position += velocity * m_data.front;
			break;
		case eCameraMovement::kBackward:
			m_data.position -= velocity * m_data.front;
			break;
		case eCameraMovement::kLeft:
			m_data.position -= velocity * m_data.right;
			break;
		case eCameraMovement::kRight:
			m_data.position += velocity * m_data.right;
			break;
		case eCameraMovement::kUp:
			m_data.position += velocity * m_data.up;
			break;
		case eCameraMovement::kDown:
			m_data.position -= velocity * m_data.up;
			break;
		}

		UpdateCameraVectors();
	}

	// protected functions
	void Camera::UpdateCameraVectors()
	{
		// Calculate camera front vector from Euler rotations
		glm::vec3 camera_direction;
		camera_direction.x = cos(glm::radians(m_data.yaw)) * cos(glm::radians(m_data.pitch));
		camera_direction.y = sin(glm::radians(m_data.pitch));
		camera_direction.z = sin(glm::radians(m_data.yaw)) * cos(glm::radians(m_data.pitch));
		m_data.front = glm::normalize(camera_direction);

		m_data.right = glm::normalize(glm::cross(m_data.front, TY_DEFAULT_WORLD_UP));
		m_data.up = glm::normalize(glm::cross(m_data.right, m_data.front));
	}

}