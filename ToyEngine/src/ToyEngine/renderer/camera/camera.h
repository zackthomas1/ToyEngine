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
		eCameraType type;

		glm::vec3 position;
		glm::vec3 front;
		glm::vec3 right;
		glm::vec3 up;

		// Perspective camera attributes
		float fov;

		// Orthographic camera attributes
		float left_bound, right_bound, bottom_bound, top_bound;
		
		// Camera attributes
		float pitch, yaw, znear, zfar;
		
		float movementSpeed;
		float mouseSensitivity;

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
		//virtual void Update();
		virtual void UpdatePosition(eCameraMovement direction, float time_step);
		virtual void UpdateLookDirection(float x_offset, float y_offset);
		virtual void UpdateFOV(float y_offset);
	private:
		virtual void UpdateCameraVectors();
	private: 
		CameraProps m_data;
		eCameraMovement movement_update;
	};

}
