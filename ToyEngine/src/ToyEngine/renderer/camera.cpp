#include "pch.h"
#include "camera.h"
#include "ToyEngine/application.h"

namespace ToyEngine
{
	Camera::Camera(eCameraType type, const CameraProps& props) : data_(props)
	{
		data_.type = type;
		TY_CORE_INFO("Create {}", CameraTypeToString(data_.type));
		UpdateCameraVectors();
	}

	Camera::Camera(const CameraProps& props) : data_(props)
	{
		TY_CORE_INFO("Create {}", CameraTypeToString(data_.type));
		UpdateCameraVectors();
	}

	glm::mat4 Camera::GetViewMatrix() const
	{
		return glm::lookAt(data_.position, data_.position + data_.front, data_.up);
	}
	
    // Returns the projection matrix for the camera based on its type and properties.
    glm::mat4 Camera::GetProjectionMatrix() const
    {
        // Ensure the camera type is valid before proceeding.
        TY_CORE_ASSERT(IsValidCameraType(data_.type), "eCameraType enum invalid");
        switch (data_.type)
        {
        case eCameraType::kFlyCamera: {
            // For perspective (fly) camera, calculate aspect ratio from the window.
            float aspect = Application::AccessWindow().GetAspectRatio();
            // Return a perspective projection matrix using field of view, aspect ratio, near and far planes.
            return glm::perspective(glm::radians(data_.fov), aspect, data_.znear, data_.zfar);
        };
        case eCameraType::kOrthographicCamera: {
            // For orthographic camera, use the defined bounds and near/far planes.
            return glm::ortho(data_.left_bound, data_.right_bound, data_.bottom_bound, data_.top_bound, data_.znear, data_.zfar);
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
		data_.movementSpeed = speed;
	}
	void Camera::SetMouseSensitivity(float sensitivity)
	{
		data_.mouseSensitivity = sensitivity;
	}
	void Camera::SetNear(float znear)
	{
		data_.znear = znear;
	}
	void Camera::SetFar(float zfar)
	{
		data_.zfar = zfar;
	}

	// call-back
	void Camera::UpdatePosition(eCameraMovement direction, float time_step)
	{
		float velocity = data_.movementSpeed * time_step;
		switch (direction)
		{
		case eCameraMovement::kForward:
			data_.position += velocity * data_.front;
			break;
		case eCameraMovement::kBackward:
			data_.position -= velocity * data_.front;
			break;
		case eCameraMovement::kLeft:
			data_.position -= velocity * data_.right;
			break;
		case eCameraMovement::kRight:
			data_.position += velocity * data_.right;
			break;
		case eCameraMovement::kUp:
			data_.position += velocity * data_.up;
			break;
		case eCameraMovement::kDown:
			data_.position -= velocity * data_.up;
			break;
		}
		UpdateCameraVectors();
	}

	void Camera::UpdateLookDirection(float x_offset, float y_offset)
	{
		x_offset *= data_.mouseSensitivity;
		y_offset *= data_.mouseSensitivity;

		data_.yaw += x_offset;
		data_.pitch -= y_offset;

		if (data_.pitch > 89.0f)
			data_.pitch = 89.0f;
		else if (data_.pitch < -89.0f)
			data_.pitch = -89.0f;
		UpdateCameraVectors();
	}

	void Camera::UpdateFOV(float y_offset)
	{
		data_.fov -= y_offset;
		if (data_.fov < 1.0f)
			data_.fov = 1.0f;
		else if (data_.fov > 90.0f)
			data_.fov = 90.0f;
	}

    // Updates the camera's direction vectors (front, right, up) based on the current yaw and pitch angles.
    // This is typically called after changing the camera's orientation or position.
    void Camera::UpdateCameraVectors()
    {
        // Calculate the new front vector from the camera's Euler angles (yaw and pitch).
        // The front vector points in the direction the camera is facing.
        glm::vec3 camera_direction;
        camera_direction.x = cos(glm::radians(data_.yaw)) * cos(glm::radians(data_.pitch));
        camera_direction.y = sin(glm::radians(data_.pitch));
        camera_direction.z = sin(glm::radians(data_.yaw)) * cos(glm::radians(data_.pitch));
        data_.front = glm::normalize(camera_direction);

        // Recalculate the right vector as the cross product of the front vector and the world's up vector.
        // This ensures the right vector is always perpendicular to the front and up vectors.
        data_.right = glm::normalize(glm::cross(data_.front, TY_DEFAULT_WORLD_UP));

        // Recalculate the up vector as the cross product of the right and front vectors.
        // This ensures the up vector is always perpendicular to the front and right vectors.
        data_.up = glm::normalize(glm::cross(data_.right, data_.front));
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