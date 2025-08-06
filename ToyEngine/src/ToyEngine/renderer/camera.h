#pragma once
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include "ToyEngine/enum.h"

namespace ToyEngine
{

 	class CameraController;	// Forward declaration

	constexpr glm::vec3 TY_DEFAULT_WORLD_UP = glm::vec3(0.0f, 1.0f, 0.0f);
	
	struct CameraProps
	{
		// The type of camera (e.g., perspective, orthographic, fly camera, etc.)
		eCameraType type;

		glm::vec3 position;	// Camera GetPosition in world space
		glm::vec3 front;	// The direction the camera is facing
		glm::vec3 right;	// The right vector relative to the camera's orientation
		glm::vec3 up;		// The up vector relative to the camera's orientation

		float aspect_ratio;
		float fov;	// Field of view for perspective cameras (in degrees)

		// Orthographic camera bounds (left, right, bottom, top)
		float left_bound, right_bound, bottom_bound, top_bound;

		// Camera orientation and clipping planes
		float pitch;	// Pitch: rotation around the X axis (up/down look)
		float yaw;		// Yaw: rotation around the Y axis (left/right look)
		float znear;	// Near clipping plane distance
		float zfar;		// Far clipping plane distance

		CameraProps(
			eCameraType type			= eCameraType::kPerspective,
			const glm::vec3& position	= glm::vec3(0.0f, 0.0f, 3.0f),
			const glm::vec3& front		= glm::vec3(0.0f, 0.0f, -1.0f),
			const glm::vec3& right		= glm::vec3(0.0f),
			const glm::vec3& up			= glm::vec3(0.0f, 1.0f, 0.0f),
			float aspect_ratio	= static_cast<float>(TY_DEFAULT_WINDOW_WIDTH) / static_cast<float>(TY_DEFAULT_WINDOW_HEIGHT),
			float fov			= 45.0f,
			float left_bound = -4.0f, float right_bound = 4.0f, float bottom_bound = -3.0f, float top_bound = 3.0f,
			float pitch = 0.0f, float yaw = -90.0f,	float znear = 0.1f, float zfar = 100.0f)
				: type(type), position(position), front(front), right(right), up(up),
				aspect_ratio(aspect_ratio), fov(fov),
				left_bound(left_bound), right_bound(right_bound), bottom_bound(bottom_bound), top_bound(top_bound),
				pitch(pitch), yaw(yaw), znear(znear), zfar(zfar)
		{}
	};

	class Camera
	{
	public:
		friend class CameraController;

		~Camera() {}

		const CameraProps& GetProps() const { return cam_props_; }
		glm::mat4 GetViewMatrix() const;
		glm::mat4 GetProjectionMatrix() const;
		eCameraType GetCameraType() const { return cam_props_.type; }
		
		void SetNear(float znear) { cam_props_.znear = znear; }
		void SetFar(float zfar) { cam_props_.zfar = zfar; }
		void SetCameraType(eCameraType type) { cam_props_.type = type; }
		void SetAspectRatio(float aspect_ratio) { cam_props_.aspect_ratio = aspect_ratio; };
		void SetFOV(float fov) { cam_props_.fov = glm::clamp(fov, 1.0f, 179.0f); }
		void SetPosition(const glm::vec3& position) { cam_props_.position = position; }
		void SetOrientation(glm::mat4 R);
		void SetOrientation(float yaw, float pitch);

	protected:
		Camera(const CameraProps& props = CameraProps());
	private:
		void UpdateCameraVectors();
		static char* CameraTypeToString(eCameraType type);
		static inline bool IsValidCameraType(eCameraType type);
	private: 
		CameraProps cam_props_;
	};
}
