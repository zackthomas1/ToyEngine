#pragma once
#include "ToyEngine/enum.h"
#include "ToyEngine/event.h"
#include "ToyEngine/renderer/camera.h"
#include "ToyEngine/services/time_step.h"
#include "ToyEngine/services/locator.h"

namespace ToyEngine
{
	class ICameraStrategy; 
	class FlyCameraStrategy;

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

	class CameraController
	{
	public:
		CameraController(const CameraControllerProps& props = CameraControllerProps());
		~CameraController() {}

		void Update(const TimeStep& time_step);
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
		Camera camera_;
	};

	class ICameraStrategy
	{
	public:
		friend class CameraController;
		virtual ~ICameraStrategy() {}

	protected:
		ICameraStrategy() {}

		virtual void Update(Camera& camera, const CameraControllerProps& props, const TimeStep& time_step) {}
		virtual bool OnMouseScroll(Camera& camera, const CameraControllerProps& props, EventVerticalScroll& e) { return false; }
		virtual bool OnMouseMove(Camera& camera, const CameraControllerProps& props, EventCursorPos& e) { return false; }
	};

	class FlyCameraStrategy : public ICameraStrategy
	{
	public: 
		friend class CameraController;
	protected:
		FlyCameraStrategy() {}

		virtual void Update(Camera& camera, const CameraControllerProps& props, const TimeStep& time_step)		override;
		virtual bool OnMouseScroll(Camera& camera, const CameraControllerProps& props, EventVerticalScroll& e)	override;
		virtual bool OnMouseMove(Camera& camera, const CameraControllerProps& props, EventCursorPos& e)			override;
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
		virtual bool OnMouseScroll(Camera& camera, const CameraControllerProps& props, EventVerticalScroll& e)	override;
		virtual bool OnMouseMove(Camera& camera, const CameraControllerProps& props, EventCursorPos& e)			override;
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

		virtual void Update(Camera& camera, const CameraControllerProps& props, const TimeStep& time_step)		override;
	};

}