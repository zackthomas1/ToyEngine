#pragma once
#include "ToyEngine/enum.h"
#include "ToyEngine/event.h"
#include "ToyEngine/renderer/camera.h"

namespace ToyEngine
{
	// forward declare
	class InputPoll;

	struct CameraControllerProps
	{
		eCameraControllerType type;
		float movement_speed;		// Camera movement speed (units per second)
		float mouse_sensitivity;	// Mouse sensitivity for camera rotation
		float scroll_sensitivity;	// Mouse sensitivity for camera scroll

		CameraControllerProps(
			eCameraControllerType type = eCameraControllerType::kFly,
			float movementSpeed = 10.0f, float mouseSensitivity = 0.01f, float scrollSensitivity = 0.1f)
				: type(type), 
				movement_speed(movementSpeed), 
				mouse_sensitivity(mouseSensitivity), 
				scroll_sensitivity(scrollSensitivity)
		{}
	};

	class ICameraStrategy
	{
	public:
		friend class CameraController;
		virtual ~ICameraStrategy() {}

	protected:
		ICameraStrategy() {}

		virtual void Update(Camera& camera, const CameraControllerProps& props, const InputPoll& input, float time_delta) {}
		virtual bool OnMouseScroll(Camera& camera, const CameraControllerProps& props, const InputPoll& input, EventVerticalScroll& e) { return false; }
		virtual bool OnMouseMove(Camera& camera, const CameraControllerProps& props, const InputPoll& input, EventCursorPos& e) { return false; }
	};

	class CameraController
	{
	public:
		CameraController(const InputPoll& input, const CameraControllerProps& props = CameraControllerProps());

		void Update(float time_delta);
		bool OnEvent(Event& e);
		
		void OnResize(float width, float height);

		const Camera& GetCamera() const { return camera_; }
		const CameraControllerProps& GetProps() const { return ctrl_props_; }

		void SetMovementSpeed(float speed) { ctrl_props_.movement_speed = speed; }
		void SetMouseSensitivity(float sensitivity) { ctrl_props_.mouse_sensitivity = sensitivity; }
		void SetScrollSensitivity(float sensitivity) { ctrl_props_.scroll_sensitivity = sensitivity; }
	private:
		bool OnWindowResize(EventWindowResize& e);
	private:
		CameraControllerProps ctrl_props_;
		Scope<ICameraStrategy> strategy_;
		const InputPoll& input_;
		Camera camera_;
	};

	class FlyCameraStrategy : public ICameraStrategy
	{
	public: 
		friend class CameraController;
	protected:
		FlyCameraStrategy() {}

		virtual void Update(Camera& camera, const CameraControllerProps& props, const InputPoll& input, float time_delta)		override;
		virtual bool OnMouseScroll(Camera& camera, const CameraControllerProps& props, const InputPoll& input, EventVerticalScroll& e)	override;
		virtual bool OnMouseMove(Camera& camera, const CameraControllerProps& props, const InputPoll& input, EventCursorPos& e)			override;
	};

	constexpr float ORBIT_RADIUS_MIN = 0.5f;
	constexpr float ORBIT_RADIUS_MAX = 20.0f;

	class OrbitCameraStrategy : public ICameraStrategy
	{
	public:
		friend class CameraController;
	protected:
		OrbitCameraStrategy(float orbit_radius)
			: orbit_radius_(orbit_radius), target_point_(glm::vec3(0.0f))
		{}
		virtual bool OnMouseScroll(Camera& camera, const CameraControllerProps& props, const InputPoll& input, EventVerticalScroll& e)	override;
		virtual bool OnMouseMove(Camera& camera, const CameraControllerProps& props, const InputPoll& input, EventCursorPos& e)			override;
	private:
		float orbit_radius_;
		glm::vec3 target_point_; 
	};

	class OrthoCameraStrategy : public ICameraStrategy
	{
	public:
		friend class CameraController;
	protected:
		OrthoCameraStrategy() {}

		virtual void Update(Camera& camera, const CameraControllerProps& props, const InputPoll& input, float time_delta)		override;
	};

}