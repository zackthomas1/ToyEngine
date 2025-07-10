#pragma once
#include "ToyEngine/enum.h"
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace ToyEngine
{
	constexpr glm::vec3 TY_DEFAULT_WORLD_UP = glm::vec3(0.0f, 1.0f, 0.0f);

	struct CameraProps
	{
					// The type of camera (e.g., perspective, orthographic, fly camera, etc.)
					eCameraType type;

					glm::vec3 position;	// Camera position in world space
					glm::vec3 front;	// The direction the camera is facing
					glm::vec3 right;	// The right vector relative to the camera's orientation
					glm::vec3 up;		// The up vector relative to the camera's orientation

					float fov;			// Field of view for perspective cameras (in degrees)

					// Orthographic camera bounds (left, right, bottom, top)
					float left_bound, right_bound, bottom_bound, top_bound;
					
					// Camera orientation and clipping planes
					float pitch;			// Pitch: rotation around the X axis (up/down look)
					float yaw;				// Yaw: rotation around the Y axis (left/right look)
					float znear;			// Near clipping plane distance
					float zfar;				// Far clipping plane distance
					float movementSpeed;	// Camera movement speed (units per second)
					float mouseSensitivity;	// Mouse sensitivity for camera rotation

					CameraProps(
						eCameraType type = eCameraType::kFlyCamera,
						glm::vec3& position = glm::vec3(0.0f, 0.0f, 3.0f),
						glm::vec3& front = glm::vec3(0.0f, 0.0f, -1.0f),
						glm::vec3& right = glm::vec3(0.0f),
						glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f),
						float fov = 45.0f,
						float left_bound = -4.0f, float right_bound = 4.0f, float bottom_bound = -3.0f, float top_bound = 3.0f,
						float pitch = 0.0f, float yaw = -90.0f,	float znear = 0.1f, float zfar = 100.0f,
						float movementSpeed = 10.0f, float mouseSensitivity = 0.1f
					) : type(type), position(position), front(front), right(right), up(up),
						fov(fov),
						left_bound(left_bound), right_bound(right_bound), bottom_bound(bottom_bound), top_bound(top_bound),
						pitch(pitch), yaw(yaw), znear(znear), zfar(zfar),
						movementSpeed(movementSpeed), mouseSensitivity(mouseSensitivity)
					{}
	};

	class Camera
	{
	public:
		Camera(eCameraType type, const CameraProps& props = CameraProps());
		Camera(const CameraProps& props = CameraProps());
		virtual ~Camera() {}

		// getter/setters
		virtual glm::mat4 GetViewMatrix() const;
		virtual glm::mat4 GetProjectionMatrix() const;

		void SetMovementSpeed(float speed); 
		void SetMouseSensitivity(float sensitivity); 
		void SetNear(float znear);
		void SetFar(float zfar);

		// update methods
		virtual void UpdatePosition(eCameraMovement direction, float time_step);
		virtual void UpdateLookDirection(float x_offset, float y_offset);
		virtual void UpdateFOV(float y_offset);
	private:
		virtual void UpdateCameraVectors();
		static char* CameraTypeToString(eCameraType type);
		static inline bool IsValidCameraType(eCameraType type);
	private: 
		CameraProps m_data;
	};

}
