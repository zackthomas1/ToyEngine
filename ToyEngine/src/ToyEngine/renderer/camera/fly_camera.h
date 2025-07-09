#pragma once
#include "camera.h"

namespace ToyEngine
{
	class FlyCamera : public Camera
	{
	public:
		FlyCamera() {}
		FlyCamera(const CameraProps& props) : Camera(props) {}
		~FlyCamera() {}

		// getter/setters
		glm::mat4 GetProjectionMatrix() const;

		// call-back
		void UpdateLookDirection(float x_offset, float y_offset, GLboolean constrainPitch = TRUE);
		void UpdateFOV(float y_offset);
	};
}
