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
		// Alternative: using lambda to capture parameters and pass them to function
		//dispatcher.Dispatch<EventVerticalScroll>([this](EventVerticalScroll& event) {
		//	return strategy_->OnMouseScroll(camera_, ctrl_props_, event);
		//});
		dispatcher.Dispatch<EventVerticalScroll>(TY_BINDFN_ARGS(strategy_->OnMouseScroll, camera_, ctrl_props_));
		dispatcher.Dispatch<EventCursorPos>(TY_BINDFN_ARGS(strategy_->OnMouseMove, camera_, ctrl_props_));
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
		ToyEngine::InputPoll& input = ToyEngine::Locator::InputPollService();

		if ((input.Key(eKeyCode::kKeyLCtrl) != eKeyState::kRelease) &&
			(input.Mouse(eMouseCode::kMouseMiddle) != eKeyState::kRelease) ) 
		{
			// Push in/out
			glm::vec3 position = camera.GetProps().position;
			glm::vec3 view_dir = camera.GetProps().front;
			float velocity = e.GetXOffset() * props.mouseSensitivity;
			camera.SetPosition(position + (view_dir * velocity));
		}
		else if ((input.Key(eKeyCode::kKeyLShift) != eKeyState::kRelease) &&
			(input.Mouse(eMouseCode::kMouseMiddle) != eKeyState::kRelease))
		{
			// Pan
			glm::vec3 cam_right = camera.GetProps().right;
			glm::vec3 cam_up	= camera.GetProps().up;

			camera.SetPosition(camera.GetProps().position + (cam_right * ((float)e.GetXOffset() * props.mouseSensitivity)));
			camera.SetPosition(camera.GetProps().position - (cam_up * ((float)e.GetYOffset() * props.mouseSensitivity)));
		}
		else if ((input.Key(eKeyCode::kKeyLAlt) != eKeyState::kRelease) &&
			(input.Mouse(eMouseCode::kMouseMiddle) != eKeyState::kRelease))
		{
			// rotate snap to orthogonal planes
			TY_CORE_WARN("TODO:: Implement orbit camera rotation snapping to orthogonal planes.");
		}
		else if ((input.Mouse(eMouseCode::kMouseMiddle) != eKeyState::kRelease))
		{
			// rotate
			float x_offset = static_cast<float>(e.GetXOffset()) * props.mouseSensitivity;
			float y_offset = static_cast<float>(e.GetYOffset()) * props.mouseSensitivity;
			
			// Get current camera position and calculate target point
			glm::vec3 current_pos = camera.GetProps().position;
			glm::vec3 current_front = camera.GetProps().front;

			// Calculate distance to target (or use a default if not set)
			static glm::vec3 target_point = glm::vec3(0.0f, 0.0f, 0.0f); // Default target at origin
			static float orbit_radius = glm::length(current_pos - target_point);
			
			//// If radius is too small, set a default
			//if (orbit_radius < 0.1f) {
			//	orbit_radius = 5.0f;
			//}

			// Calculate current spherical coordinates relative to target
			glm::vec3 relative_pos = current_pos - target_point;

			// Current azimuth (horizontal angle around Y-axis)
			float current_azimuth = atan2(relative_pos.z, relative_pos.x);

			// Current elevation (vertical angle from XZ plane)
			float current_elevation = asin(glm::clamp(relative_pos.y / orbit_radius, -1.0f, 1.0f));

			// Apply mouse delta to angles
			float new_azimuth = current_azimuth + glm::radians(x_offset);
			float new_elevation = current_elevation - glm::radians(y_offset); // Inverted for natural feel

			// Clamp elevation to prevent gimbal lock
			const float max_elevation = glm::radians(89.0f);
			new_elevation = glm::clamp(new_elevation, -max_elevation, max_elevation);

			// Calculate new position using spherical coordinates
			glm::vec3 new_position;
			new_position.x = target_point.x + orbit_radius * cos(new_elevation) * cos(new_azimuth);
			new_position.y = target_point.y + orbit_radius * sin(new_elevation);
			new_position.z = target_point.z + orbit_radius * cos(new_elevation) * sin(new_azimuth);

			// Calculate new front vector (always pointing toward target)
			glm::vec3 new_front = glm::normalize(target_point - new_position);

			// Calculate yaw and pitch for the camera's SetOrientation method
			float yaw = glm::degrees(atan2(new_front.z, new_front.x));
			float pitch = glm::degrees(asin(-new_front.y));

			// Update camera position and orientation
			camera.SetPosition(new_position);
			camera.SetOrientation(yaw, pitch);

		}
		return true;
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