#include "pch.h"
#include "camera.h"
#include "ToyEngine/application.h"

namespace ToyEngine
{
  Camera::Camera(const CameraProps& props) : cam_props_(props)
  {
    TY_CORE_INFO("Create {}", CameraTypeToString(cam_props_.type));
    UpdateCameraVectors();
  }

  glm::mat4 Camera::GetViewMatrix() const
  {
    return glm::lookAt(cam_props_.position, cam_props_.position + cam_props_.front, cam_props_.up);
  }
  
  // Returns the projection matrix for the camera based on its type and properties.
  glm::mat4 Camera::GetProjectionMatrix() const
  {
    // Ensure the camera type is valid before proceeding.
    TY_CORE_ASSERT(IsValidCameraType(cam_props_.type), "eCameraType enum invalid");
    switch (cam_props_.type)
    {
    case eCameraType::kPerspective: {
      // For perspective (fly) camera, calculate aspect ratio from the window.
      // Return a perspective projection matrix using field of view, aspect ratio, near and far planes.
      return glm::perspective(glm::radians(cam_props_.fov), cam_props_.aspect_ratio, cam_props_.znear, cam_props_.zfar);
    };
    case eCameraType::kOrthographic: {
      // For orthographic camera, use the defined bounds and near/far planes.
      return glm::ortho(cam_props_.left_bound, cam_props_.right_bound, cam_props_.bottom_bound, cam_props_.top_bound, cam_props_.znear, cam_props_.zfar);
    }
    default:
      // Log an error if the camera type is unknown.
      TY_CORE_ERROR("Camera type unknown: Unable to determine GetProjectionMatrix value.");
      break;
    }
    // Return identity matrix as a fallback if the camera type is invalid.
    return glm::mat4(1.0f);
  }

  void Camera::SetOrientation(glm::mat4 R) {
    cam_props_.front	= glm::vec3(R * glm::vec4(cam_props_.front, 0.0f));
    cam_props_.right	= glm::vec3(R * glm::vec4(cam_props_.right, 0.0f));
    cam_props_.up		= glm::vec3(R * glm::vec4(cam_props_.up, 0.0f));

    /*Update yaw and pitch based on updated orientation
    cam_props_.yaw		= glm::degrees(atan2(-pos.z, -pos.x));
    cam_props_.pitch	= glm::degrees(asin(-pos.y));
    TY_INFO("yaw: {} pitch: {}", yaw, pitch);
    */
  }

  void Camera::SetOrientation(float yaw, float pitch)
  {
    cam_props_.yaw = yaw;
    cam_props_.pitch = pitch; 
    UpdateCameraVectors();
  }

  // Updates the camera's direction vectors (front, right, up) based on the current yaw and pitch angles.
  // This is typically called after changing the camera's orientation or GetPosition.
  void Camera::UpdateCameraVectors()
  {
    // Calculate the new front vector from the camera's Euler angles (yaw and pitch).
    // The front vector points in the direction the camera is facing.
    glm::vec3 camera_direction;
    camera_direction.x = cos(glm::radians(cam_props_.yaw)) * cos(glm::radians(cam_props_.pitch));
    camera_direction.y = sin(glm::radians(cam_props_.pitch));
    camera_direction.z = sin(glm::radians(cam_props_.yaw)) * cos(glm::radians(cam_props_.pitch));
    cam_props_.front = glm::normalize(camera_direction);

    // Recalculate the right vector as the cross product of the front vector and the world's up vector.
    // This ensures the right vector is always perpendicular to the front and up vectors.
    cam_props_.right = glm::normalize(glm::cross(cam_props_.front, TY_DEFAULT_WORLD_UP));

    // Recalculate the up vector as the cross product of the right and front vectors.
    // This ensures the up vector is always perpendicular to the front and right vectors.
    cam_props_.up = glm::normalize(glm::cross(cam_props_.right, cam_props_.front));
  }

  char* Camera::CameraTypeToString(eCameraType type)
  {
    switch (type) {
    case eCameraType::kPerspective: return "Fly Camera";
    case eCameraType::kOrthographic: return "Orthographic Camera";
    default: return "Unknown Camera Type";
    }
  }

  // Bitwise validity check:
  inline bool Camera::IsValidCameraType(eCameraType type)
  {
    constexpr uint32_t kAllFlags = static_cast<uint32_t>(eCameraType::kPerspective) |
      static_cast<uint32_t>(eCameraType::kOrthographic);
    uint32_t t = static_cast<uint32_t>(type);
    
    // (t & kAllFlags) == t: Checks if all bits set in t are also set in kAllFlags, i.e., t is a valid flag.
    // If type is kPerspective(1): kAllFlags = 1 | 2 = 3 (binary 11) -> t & kAllFlags = 1 & 3 = 1, which equals t(valid)
    // If type is 4 (not defined):	t & kAllFlags = 4 & 3 = 0, which does not equal t(invalid)
    return t != 0 && (t & kAllFlags) == t;
  }
}