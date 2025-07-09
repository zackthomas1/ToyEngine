#include "pch.h"
#include "orthographic_camera.h"

namespace ToyEngine 
{
	// getters/setters
	glm::mat4 OrthographicCamera::GetProjectionMatrix() const
	{
		return glm::ortho(m_data.left_bound, m_data.right_bound, m_data.bottom_bound, m_data.top_bound, m_data.znear, m_data.zfar);
	}
}