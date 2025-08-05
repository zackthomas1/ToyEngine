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
		float x_offset = static_cast<float>(e.GetOffset().x) * props.mouseSensitivity;
		float y_offset = static_cast<float>(e.GetOffset().y) * props.mouseSensitivity;
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
	bool OrbitCameraStrategy::OnMouseMove(Camera& camera, const CameraControllerProps& props, EventCursorPos& e)
	{
		ToyEngine::InputPoll& input = ToyEngine::Locator::InputPollService();

		if ((input.Key(eKeyCode::kKeyLCtrl) != eKeyState::kRelease) &&
			(input.Mouse(eMouseCode::kMouseMiddle) != eKeyState::kRelease) ) 
		{
			// Push in/out
			glm::vec3 position = camera.GetProps().position;
			glm::vec3 view_dir = camera.GetProps().front;
			float zoom_delta = e.GetOffset().y * props.mouseSensitivity;
			camera.SetPosition(position + (view_dir * zoom_delta));
		}
		else if ((input.Key(eKeyCode::kKeyLShift) != eKeyState::kRelease) &&
			(input.Mouse(eMouseCode::kMouseMiddle) != eKeyState::kRelease))
		{
			// Pan
			glm::vec3 cam_right = camera.GetProps().right;
			glm::vec3 cam_up	= camera.GetProps().up;

			camera.SetPosition(camera.GetProps().position + (cam_right * ((float)e.GetOffset().x * props.mouseSensitivity)));
			camera.SetPosition(camera.GetProps().position - (cam_up * ((float)e.GetOffset().y * props.mouseSensitivity)));
		}
		else if ((input.Key(eKeyCode::kKeyLAlt) != eKeyState::kRelease) &&
			(input.Mouse(eMouseCode::kMouseMiddle) != eKeyState::kRelease))
		{
			// rotate snap to orthogonal planes
			TY_CORE_WARN("TODO:: Implement orbit camera rotation snapping to orthogonal planes.");
		}
		else if ((input.Mouse(eMouseCode::kMouseMiddle) != eKeyState::kRelease))
		{
			glm::vec3 p_ndc			= glm::vec3(e.GetNDCCoordPrev(),0.0);
			glm::vec3 p_prim_ndc	= glm::vec3(e.GetNDCCoord(),0.0);
			TY_CORE_INFO("NDC: ({},{})", p_prim_ndc.x, p_prim_ndc.y);

			p_ndc.z			= glm::sqrt(1.0f - glm::min((p_ndc.x * p_ndc.x) + (p_ndc.y * p_ndc.y), 1.0f));
			p_prim_ndc.z	= glm::sqrt(1.0f - glm::min((p_prim_ndc.x * p_prim_ndc.x) + (p_prim_ndc.y * p_prim_ndc.y), 1.0f));
			
			p_ndc		= glm::normalize(p_ndc);
			p_prim_ndc	= glm::normalize(p_prim_ndc);
			
			float cos_theta = glm::clamp(glm::dot(p_ndc, p_prim_ndc), -1.0f, 1.0f);
			float theta		= glm::min(glm::acos(cos_theta), 1.0f);
			float sin_theta = glm::sin(theta);
			glm::vec3 u		= glm::normalize(glm::cross(p_ndc, p_prim_ndc));
			TY_CORE_INFO("theta: {}", theta );
			TY_CORE_INFO("u: ({},{},{})", u.x, u.y, u.z);

			//glm::mat4 R({
			//	(u.x * u.x) + (1.0f - (u.x * u.x)) * cos_theta,			(u.x * u.y) * (1.0f - cos_theta) - (u.z * sin_theta),	(u.x * u.z) * (1.0f - cos_theta) + (u.y * sin_theta),	0.0f,
			//	(u.y * u.x) * (1.0f - cos_theta) + (u.z * sin_theta),	(u.y * u.y) + (1.0f - (u.y * u.y)) * cos_theta,			(u.y * u.z) * (1.0f - cos_theta) - (u.x * sin_theta),	0.0f,
			//	(u.z * u.x) * (1.0f - cos_theta) - (u.y * sin_theta),	(u.z * u.y) * (1.0f - cos_theta) + (u.x * sin_theta),	(u.z * u.z) + (1.0f - (u.z * u.z)) * cos_theta,			0.0f,
			//	0.0f,													0.0f,													0.0f,													1.0f,
			//});
			glm::quat rotation_quat = glm::angleAxis(2.0f * theta, u);
			glm::mat4 R = glm::mat4_cast(rotation_quat);

			glm::vec3 pos = glm::vec3(R * glm::vec4(camera.GetProps().position, 0.0f));
			camera.SetPosition(pos);
			TY_CORE_INFO("pos: ({},{},{})", pos.x, pos.y, pos.z);

			camera.SetOrientation(R);

			//float yaw	= glm::degrees(atan2(-pos.z, -pos.x));
			//float pitch	= glm::degrees(asin(-pos.y)); 
			//camera.SetOrientation(yaw, pitch);
			//TY_CORE_INFO("yaw: {}", yaw);
			//TY_CORE_INFO("pitch: {}", pitch);
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
		TY_CORE_WARN("TODO:: Implement  OrthoCameraStrategy::OnMouseScroll");
		return false;
	}

	bool OrthoCameraStrategy::OnMouseMove(Camera& camera, const CameraControllerProps& props, EventCursorPos& e)
	{
		TY_CORE_WARN("TODO:: Implement  OrthoCameraStrategy::OnMouseMove");
		return false;
	}

}