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
		float movementSpeed;	// Camera movement speed (units per second)
		float mouseSensitivity;	// Mouse sensitivity for camera rotation
		
		CameraControllerProps(
			eCameraControllerType type = eCameraControllerType::kFly,
			float movementSpeed = 10.0f, float mouseSensitivity = 0.1f)
				: type(type), movementSpeed(movementSpeed), mouseSensitivity(mouseSensitivity)
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

		Camera& GetCamera() { return camera_; }
		const Camera& GetCamera() const { return camera_; }

		void SetMovementSpeed(float speed) { ctrl_props_.movementSpeed = speed; }
		void SetMouseSensitivity(float sensitivity) { ctrl_props_.mouseSensitivity = sensitivity; }
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

		virtual void Update(Camera& camera, const CameraControllerProps& props, const TimeStep& time_step) = 0;
		virtual bool OnMouseScroll(Camera& camera, const CameraControllerProps& props, EventVerticalScroll& e) = 0;
		virtual bool OnMouseMove(Camera& camera, const CameraControllerProps& props, EventCursorPos& e) = 0;
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

	class OrbitCameraStrategy : public ICameraStrategy
	{
	public:
		friend class CameraController;
	protected:
		OrbitCameraStrategy() {}

		virtual void Update(Camera& camera, const CameraControllerProps& props, const TimeStep& time_step)		override;
		virtual bool OnMouseScroll(Camera& camera, const CameraControllerProps& props, EventVerticalScroll& e)	override;
		virtual bool OnMouseMove(Camera& camera, const CameraControllerProps& props, EventCursorPos& e)			override;
	};

	class OrthoCameraStrategy : public ICameraStrategy
	{
	public:
		friend class CameraController;
	protected:
		OrthoCameraStrategy() {}

		virtual void Update(Camera& camera, const CameraControllerProps& props, const TimeStep& time_step)		override;
		virtual bool OnMouseScroll(Camera& camera, const CameraControllerProps& props, EventVerticalScroll& e)	override;
		virtual bool OnMouseMove(Camera& camera, const CameraControllerProps& props, EventCursorPos& e)			override;
	};

}