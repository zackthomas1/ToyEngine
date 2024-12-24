#pragma once
#include "camera.h"

namespace ToyEngine
{
	// default camera values
	const float kLeftBound = -4.0f, kRightBound = 4.0f,
		kBottomBound = -3.0f, kTopBound = 3.0;

	class OrthographicCamera : public Camera {
		
	public:
		OrthographicCamera(glm::vec3 position = kCameraPosition, glm::vec3 up = kCameraUp,
			float pitch = kPitch, float yaw = kYaw, float znear = kZnear, float zfar = kZfar,
			float left = kLeftBound, float right = kRightBound, float bottom = kBottomBound, float top = kTopBound);
		OrthographicCamera(float pos_x, float pos_y, float pos_z,
			float up_x, float up_y, float up_z,
			float pitch = kPitch, float yaw = kYaw, float znear = kZnear, float zfar = kZfar,
			float left = kLeftBound, float right = kRightBound, float bottom = kBottomBound, float top = kTopBound);
		~OrthographicCamera() {}

		// getter/setters
		glm::mat4 GetProjectionMatrix() const;

	private:
		// camera attributes
		float left_, right_, bottom_, top_;
	};
}