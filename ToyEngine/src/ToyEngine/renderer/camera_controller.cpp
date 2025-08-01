#include "pch.h"
#include "camera_controller.h"

namespace ToyEngine
{
	// Camera controller
	CameraController::CameraController(const CameraControllerProps& props)
		: ctrl_props_(props)
	{
		ToyEngine::CameraProps camera_props;
		switch (props.type)
		{
		case eCameraControllerType::kFly: {
			camera_props.type = ToyEngine::eCameraType::kPerspective;
			strategy_ = Scope<FlyCameraStrategy>(new FlyCameraStrategy());
			break;
		}
		case eCameraControllerType::kOrbit: {
			camera_props.type = ToyEngine::eCameraType::kPerspective;
			strategy_ = Scope<OrbitCameraStrategy>(new OrbitCameraStrategy());
			break;
		}
		case eCameraControllerType::kOrtho: {
			camera_props.type = ToyEngine::eCameraType::kOrthographic;
			strategy_ = Scope<OrthoCameraStrategy>(new OrthoCameraStrategy());
			break;
		}
		default:
			TY_CORE_WARN("Unknown camera controller type");
			camera_props.type = ToyEngine::eCameraType::kPerspective;
			strategy_ = Scope<FlyCameraStrategy>(new FlyCameraStrategy());
			break;
		}
		camera_ = Camera(camera_props);
	}

	void CameraController::Update(const TimeStep& time_step)
	{
		strategy_->Update(camera_, ctrl_props_, time_step);
	}

	bool CameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e); 
		dispatcher.Dispatch<EventVerticalScroll>([this](EventVerticalScroll& event) {
			return strategy_->OnMouseScroll(camera_, ctrl_props_, event);
		});
		dispatcher.Dispatch<EventCursorPos>([this](EventCursorPos& event){
			return strategy_->OnMouseMove(camera_, ctrl_props_, event);
		});
		dispatcher.Dispatch<EventWindowResize>(TY_BINDFN(CameraController::OnWindowResize));
		return true;
	}

	void ToyEngine::CameraController::OnResize(float width, float height)
	{
		camera_.SetAspectRatio(width / height);
	}

	bool CameraController::OnWindowResize(EventWindowResize& e)
	{
		camera_.SetAspectRatio(static_cast<float>(e.GetWidth()) / static_cast<float>(e.GetHeight()));
		return true;
	}

	// Fly Camera Strategy
	// --------------------
	void FlyCameraStrategy::Update(Camera& camera, const CameraControllerProps& props, const TimeStep& time_step)
	{
		float delta_time = time_step.GetTimeDelta();
		ToyEngine::InputPoll& input = ToyEngine::Locator::InputPollService();

		float velocity = props.movementSpeed * delta_time;

		if (input.Key(eKeyCode::kKeyW) != eKeyState::kRelease) // Forward
			camera.SetPosition(camera.GetProps().position + (velocity * camera.GetProps().front));
		if (input.Key(eKeyCode::kKeyS) != eKeyState::kRelease) // Backward
			camera.SetPosition(camera.GetProps().position - (velocity * camera.GetProps().front));
		if (input.Key(eKeyCode::kKeyD) != eKeyState::kRelease) // Right
			camera.SetPosition(camera.GetProps().position + (velocity * camera.GetProps().right));
		if (input.Key(eKeyCode::kKeyA) != eKeyState::kRelease) // Left
			camera.SetPosition(camera.GetProps().position - (velocity * camera.GetProps().right));
		if (input.Key(eKeyCode::kKeyE) != eKeyState::kRelease) // Up
			camera.SetPosition(camera.GetProps().position + (velocity * camera.GetProps().up));
		if (input.Key(eKeyCode::kKeyQ) != eKeyState::kRelease) // Down
			camera.SetPosition(camera.GetProps().position - (velocity * camera.GetProps().up));
	}

	bool FlyCameraStrategy::OnMouseScroll(Camera& camera, const CameraControllerProps& props, EventVerticalScroll& e)
	{
		float fov = camera.GetProps().fov - e.GetYOffset();

		if (fov < 1.0f)
			fov = 1.0f;
		else if (fov > 90.0f)
			fov = 90.0f;

		camera.SetFOV(fov);
		return true;
	}

	bool FlyCameraStrategy::OnMouseMove(Camera& camera, const CameraControllerProps& props, EventCursorPos& e)
	{
		float x_offset = static_cast<float>(e.GetXOffset()) * props.mouseSensitivity;
		float y_offset = static_cast<float>(e.GetYOffset()) * props.mouseSensitivity;
		float yaw = camera.GetProps().yaw + x_offset;
		float pitch = camera.GetProps().pitch - y_offset;

		if (pitch > 89.0f)
			pitch = 89.0f;
		else if (pitch < -89.0f)
			pitch = -89.0f;

		camera.SetOrientation(yaw, pitch);
		return true;
	}

	// Orbit Camera Strategy
	// --------------------
	void OrbitCameraStrategy::Update(Camera& camera, const CameraControllerProps& props, const TimeStep& time_step)
	{
	}

	bool OrbitCameraStrategy::OnMouseScroll(Camera& camera, const CameraControllerProps& props, EventVerticalScroll& e)
	{
		return false;
	}

	bool OrbitCameraStrategy::OnMouseMove(Camera& camera, const CameraControllerProps& props, EventCursorPos& e)
	{
		return false;
	}

	// Ortho Camera Strategy
	// --------------------

	void OrthoCameraStrategy::Update(Camera& camera, const CameraControllerProps& props, const TimeStep& time_step)
	{
	}

	bool OrthoCameraStrategy::OnMouseScroll(Camera& camera, const CameraControllerProps& props, EventVerticalScroll& e)
	{
		return false;
	}

	bool OrthoCameraStrategy::OnMouseMove(Camera& camera, const CameraControllerProps& props, EventCursorPos& e)
	{
		return false;
	}

}