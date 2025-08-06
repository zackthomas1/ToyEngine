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
			float orbit_radius = glm::distance(camera_.GetProps().position, glm::vec3(0.0f));
			strategy_ = Scope<OrbitCameraStrategy>(new OrbitCameraStrategy(orbit_radius));
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
		camera_.SetCameraType(camera_props.type);
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

		float velocity = props.movement_speed * delta_time;

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
		float x_offset = static_cast<float>(e.GetOffset().x) * props.mouse_sensitivity;
		float y_offset = static_cast<float>(e.GetOffset().y) * props.mouse_sensitivity;
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
	bool OrbitCameraStrategy::OnMouseScroll(Camera& camera, const CameraControllerProps& props, EventVerticalScroll& e)
	{
		glm::vec3 position = camera.GetProps().position;
		glm::vec3 view_dir = camera.GetProps().front;
		float zoom_delta = e.GetYOffset() * props.scroll_sensitivity;

		orbit_radius_ = glm::clamp(orbit_radius_ + zoom_delta, ORBIT_RADIUS_MIN, ORBIT_RADIUS_MAX);

		camera.SetPosition((-view_dir * orbit_radius_) + target_point_);
		return true;
	}

	bool OrbitCameraStrategy::OnMouseMove(Camera& camera, const CameraControllerProps& props, EventCursorPos& e)
	{
		ToyEngine::InputPoll& input = ToyEngine::Locator::InputPollService();

		if ((input.Key(eKeyCode::kKeyLCtrl) != eKeyState::kRelease) &&
			(input.Mouse(eMouseCode::kMouseMiddle) != eKeyState::kRelease) ) 
		{
			glm::vec3 position = camera.GetProps().position;
			glm::vec3 view_dir = camera.GetProps().front;
			float zoom_delta = e.GetOffset().y * props.mouse_sensitivity;
			
			orbit_radius_ = glm::clamp(orbit_radius_ + zoom_delta, ORBIT_RADIUS_MIN, ORBIT_RADIUS_MAX);

			camera.SetPosition((-view_dir * orbit_radius_) + target_point_);
			//TY_CORE_INFO("Orbit Radius: {:.2f} Offset Val: {:.2f}", orbit_radius_, e.GetOffset().y);
		}
		else if ((input.Key(eKeyCode::kKeyLShift) != eKeyState::kRelease) &&
			(input.Mouse(eMouseCode::kMouseMiddle) != eKeyState::kRelease))
		{
			// Pan
			glm::vec3 cam_right = camera.GetProps().right;
			glm::vec3 cam_up	= camera.GetProps().up;
				
			glm::vec2 pan_velocity(e.GetOffset().x * props.mouse_sensitivity,
						e.GetOffset().y * props.mouse_sensitivity);

			target_point_ += cam_right * pan_velocity.x;
			target_point_ -= cam_up * pan_velocity.y;
			
			camera.SetPosition(camera.GetProps().position + (cam_right * pan_velocity.x));
			camera.SetPosition(camera.GetProps().position - (cam_up * pan_velocity.y));
			//TY_CORE_INFO("Target Point: ({:.2f},{:.2f}, {:.2f}) Offset: ({:.2f},{:.2f})", target_point_.x, target_point_.y, target_point_.z, e.GetOffset().x, e.GetOffset().y);
		}
		else if ((input.Key(eKeyCode::kKeyLAlt) != eKeyState::kRelease) &&
			(input.Mouse(eMouseCode::kMouseMiddle) != eKeyState::kRelease))
		{
			// rotate snap to orthogonal planes
			TY_CORE_WARN("TODO:: Implement orbit camera rotation snapping to orthogonal planes.");
		}
		else if ((input.Mouse(eMouseCode::kMouseMiddle) != eKeyState::kRelease))
		{
			glm::vec3 p_ndc = glm::vec3(e.GetNDCCoordPrev(), 0.0);	// start pos
			glm::vec3 p_prim_ndc = glm::vec3(e.GetNDCCoord(), 0.0);	// end pos

			// Project 2D NDC to 3D sphere
			p_ndc.z			= glm::sqrt(1.0f - glm::min((p_ndc.x * p_ndc.x) + (p_ndc.y * p_ndc.y), 1.0f));
			p_prim_ndc.z	= glm::sqrt(1.0f - glm::min((p_prim_ndc.x * p_prim_ndc.x) + (p_prim_ndc.y * p_prim_ndc.y), 1.0f));
			
			p_ndc		= glm::normalize(p_ndc);
			p_prim_ndc	= glm::normalize(p_prim_ndc);
			//TY_CORE_INFO("Prev NDC:({:.2f},{:.2f},{:.2f}) NDC: ({:.2f},{:.2f},{:.2f})", p_ndc.x, p_ndc.y, p_ndc.z, p_prim_ndc.x, p_prim_ndc.y, p_prim_ndc.z);

			// Calculate rotation
			float cos_theta = glm::clamp(glm::dot(p_ndc, p_prim_ndc), -1.0f, 1.0f);
			float theta		= glm::acos(glm::min(cos_theta, 1.0f));
			glm::vec3 u		= glm::normalize(glm::cross(p_ndc, p_prim_ndc));
			//TY_CORE_INFO("theta: {:.2f} u: ({:.2f},{:.2f},{:.2f})", theta, u.x, u.y, u.z);

			// Rotation angle theta must be greater than zero and less than 180(pi)
			// rotation equal to zero will cause a division by zero when calculating
			// rotation quaterion.
			if (theta < TY_EPSILON || theta > glm::pi<float>()){
				TY_CORE_WARN("Invalid rotation angle, skipping: theta: {:.2f} u: ({:.2f},{:.2f},{:.2f})", theta, u.x, u.y, u.z);
				return true;
			}

			// quaternion implementation
			glm::quat rotation_quat = glm::angleAxis(2.0f * theta, u);
			glm::mat4 R = glm::mat4_cast(rotation_quat);

			// Apply rotation to camera position
			glm::vec3 rotated_pos = glm::vec3(R * glm::vec4(camera.GetProps().position - target_point_, 1.0f));
			camera.SetPosition(rotated_pos + target_point_);

			camera.SetOrientation(R);
		}

		return true;
	}

	// Ortho Camera Strategy
	// --------------------
	void OrthoCameraStrategy::Update(Camera& camera, const CameraControllerProps& props, const TimeStep& time_step)
	{
		float delta_time = time_step.GetTimeDelta();
		ToyEngine::InputPoll& input = ToyEngine::Locator::InputPollService();

		float velocity = props.movement_speed * delta_time;

		if (input.Key(eKeyCode::kKeyW) != eKeyState::kRelease) // Up
			camera.SetPosition(camera.GetProps().position + (velocity * camera.GetProps().up));
		if (input.Key(eKeyCode::kKeyS) != eKeyState::kRelease) // Down
			camera.SetPosition(camera.GetProps().position - (velocity * camera.GetProps().up));
		if (input.Key(eKeyCode::kKeyD) != eKeyState::kRelease) // Right
			camera.SetPosition(camera.GetProps().position + (velocity * camera.GetProps().right));
		if (input.Key(eKeyCode::kKeyA) != eKeyState::kRelease) // Left
			camera.SetPosition(camera.GetProps().position - (velocity * camera.GetProps().right));
	}
}