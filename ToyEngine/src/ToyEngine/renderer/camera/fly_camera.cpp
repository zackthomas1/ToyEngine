#include "pch.h"
#include "fly_camera.h"
#include "ToyEngine/application.h"

namespace ToyEngine{
	// getter/setter
	glm::mat4 FlyCamera::GetProjectionMatrix() const
	{
		float aspect = Application::Get().GetWindow().GetAspectRatio();
		return glm::perspective(glm::radians(m_data.fov), aspect, m_data.znear, m_data.zfar);
	}

	// call-back
	void FlyCamera::UpdateLookDirection(float x_offset, float y_offset, GLboolean constrainPitch)
	{
		x_offset *= m_data.mouseSensitivity;
		y_offset *= m_data.mouseSensitivity;

		m_data.yaw		+= x_offset;
		m_data.pitch	-= y_offset;

		if (m_data.pitch > 89.0f)
			m_data.pitch = 89.0f;
		else if (m_data.pitch < -89.0f)
			m_data.pitch = -89.0f;

		UpdateCameraVectors();
	}
	void FlyCamera::UpdateFOV(float y_offset)
	{
		m_data.fov -= y_offset;
		if (m_data.fov < 1.0f)
			m_data.fov = 1.0f;
		else if (m_data.fov > 90.0f)
			m_data.fov = 90.0f;
	}
}