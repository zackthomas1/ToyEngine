#include "pch.h"
#include "camera.h"
#include "ToyEngine/application.h"

namespace ToyEngine
{
	Camera::Camera(eCameraType type, const CameraProps& props) : m_data(props)
	{
		m_data.type = type;
		TY_CORE_INFO("Create {}", CameraTypeToString(m_data.type));
		UpdateCameraVectors();
	}

	Camera::Camera(const CameraProps& props) : m_data(props)
	{
		TY_CORE_INFO("Create {}", CameraTypeToString(m_data.type));
		UpdateCameraVectors();
	}

	glm::mat4 Camera::GetViewMatrix() const
	{
		return glm::lookAt(m_data.position, m_data.position + m_data.front, m_data.up);
	}
	
    // Returns the projection matrix for the camera based on its type and properties.
    glm::mat4 Camera::GetProjectionMatrix() const
    {
        // Ensure the camera type is valid before proceeding.
        TY_CORE_ASSERT(IsValidCameraType(m_data.type), "eCameraType enum invalid");
        switch (m_data.type)
        {
        case eCameraType::kFlyCamera: {
            // For perspective (fly) camera, calculate aspect ratio from the window.
            float aspect = Application::AccessWindow().GetAspectRatio();
            // Return a perspective projection matrix using field of view, aspect ratio, near and far planes.
            return glm::perspective(glm::radians(m_data.fov), aspect, m_data.znear, m_data.zfar);
        };
        case eCameraType::kOrthographicCamera: {
            // For orthographic camera, use the defined bounds and near/far planes.
            return glm::ortho(m_data.left_bound, m_data.right_bound, m_data.bottom_bound, m_data.top_bound, m_data.znear, m_data.zfar);
        }
        default:
            // Log an error if the camera type is unknown.
            TY_CORE_ERROR("Camera type unknown: Unable to determine GetProjectionMatrix value.");
            break;
        }
        // Return identity matrix as a fallback if the camera type is invalid.
        return glm::mat4(1.0f);
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

	void Camera::UpdateLookDirection(float x_offset, float y_offset)
	{
		x_offset *= m_data.mouseSensitivity;
		y_offset *= m_data.mouseSensitivity;

		m_data.yaw += x_offset;
		m_data.pitch -= y_offset;

		if (m_data.pitch > 89.0f)
			m_data.pitch = 89.0f;
		else if (m_data.pitch < -89.0f)
			m_data.pitch = -89.0f;
		UpdateCameraVectors();
	}

	void Camera::UpdateFOV(float y_offset)
	{
		m_data.fov -= y_offset;
		if (m_data.fov < 1.0f)
			m_data.fov = 1.0f;
		else if (m_data.fov > 90.0f)
			m_data.fov = 90.0f;
	}

    // Updates the camera's direction vectors (front, right, up) based on the current yaw and pitch angles.
    // This is typically called after changing the camera's orientation or position.
    void Camera::UpdateCameraVectors()
    {
        // Calculate the new front vector from the camera's Euler angles (yaw and pitch).
        // The front vector points in the direction the camera is facing.
        glm::vec3 camera_direction;
        camera_direction.x = cos(glm::radians(m_data.yaw)) * cos(glm::radians(m_data.pitch));
        camera_direction.y = sin(glm::radians(m_data.pitch));
        camera_direction.z = sin(glm::radians(m_data.yaw)) * cos(glm::radians(m_data.pitch));
        m_data.front = glm::normalize(camera_direction);

        // Recalculate the right vector as the cross product of the front vector and the world's up vector.
        // This ensures the right vector is always perpendicular to the front and up vectors.
        m_data.right = glm::normalize(glm::cross(m_data.front, TY_DEFAULT_WORLD_UP));

        // Recalculate the up vector as the cross product of the right and front vectors.
        // This ensures the up vector is always perpendicular to the front and right vectors.
        m_data.up = glm::normalize(glm::cross(m_data.right, m_data.front));
    }

	char* Camera::CameraTypeToString(eCameraType type)
	{
		switch (type) {
		case eCameraType::kFlyCamera: return "Fly Camera";
		case eCameraType::kOrthographicCamera: return "Orthographic Camera";
		default: return "Unknown Camera Type";
		}
	}

	// Bitwise validity check:
	inline bool Camera::IsValidCameraType(eCameraType type)
	{
		constexpr uint32_t kAllFlags = static_cast<uint32_t>(eCameraType::kFlyCamera) |
			static_cast<uint32_t>(eCameraType::kOrthographicCamera);
		uint32_t t = static_cast<uint32_t>(type);
		
		// (t & kAllFlags) == t: Checks if all bits set in t are also set in kAllFlags, i.e., t is a valid flag.
		// If type is kFlyCamera(1): kAllFlags = 1 | 2 = 3 (binary 11) -> t & kAllFlags = 1 & 3 = 1, which equals t(valid)
		// If type is 4 (not defined):	t & kAllFlags = 4 & 3 = 0, which does not equal t(invalid)
		return t != 0 && (t & kAllFlags) == t;
	}
}