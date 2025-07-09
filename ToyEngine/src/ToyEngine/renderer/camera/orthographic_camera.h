#pragma once
#include "camera.h"

namespace ToyEngine
{
	class OrthographicCamera : public Camera {
		
	public:
		OrthographicCamera() {}
		OrthographicCamera(const CameraProps& props) : Camera(props) {}
		~OrthographicCamera() {}

		// getter/setters
		glm::mat4 GetProjectionMatrix() const;

	};
}