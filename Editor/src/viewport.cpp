#include "viewport.h"

Viewport::Viewport()
{
	// Create  Framebuffer
	ToyEngine::FrameBufferProps fb_props;
	props_.framebuffer = ToyEngine::FrameBuffer::Create(fb_props);
}

void Viewport::ImGuiRender()
{
	// Viewport
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
	ImGui::Begin("Viewport");

	props_.is_focused = ImGui::IsWindowFocused();
	props_.is_hovered = ImGui::IsWindowHovered();
	ToyEngine::Application::Get().GetImGuiLayer()->BlockEvents(!props_.is_focused || !props_.is_hovered);

	// viewport properties
	props_.panel_size	= ImGui::GetContentRegionAvail();
	props_.window_min	= ImGui::GetWindowContentRegionMin();
	props_.window_max	= ImGui::GetWindowContentRegionMax();
	props_.window_pos	= ImGui::GetWindowPos();
	props_.window_size	= ImGui::GetWindowSize();

	// Check if window was resized and recreate framebuffer if needed
	if (props_.panel_size.x != props_.framebuffer->GetWidth() || props_.panel_size.y != props_.framebuffer->GetHeight()) {
		props_.framebuffer->Resize((uint32_t)props_.panel_size.x, (uint32_t)props_.panel_size.y);
		
		// Null check before calling OnResize
		if (camera_controller_) {
			camera_controller_->OnResize(props_.panel_size.x, props_.panel_size.y);
		}
	}

	ImGui::Image(props_.framebuffer->GetColorAttachment(),
		ImVec2(props_.panel_size.x, props_.panel_size.y),
		ImVec2(0, 1), ImVec2(1, 0));
	ImGui::End();
	ImGui::PopStyleVar();
}

bool Viewport::OnMouseMove(ToyEngine::EventCursorPos& cursor_event)
{
	// Null check for camera controller
	if (!camera_controller_) {
		TY_CORE_ERROR("Camera controller not set in viewport");
		return false;
	}
	// Get current viewport information from ImGui
	ImGuiIO& io = ImGui::GetIO();
	ImVec2 mouse_pos = io.MousePos;

	// Calculate viewport-relative mouse position
	glm::vec2 viewport_pos = glm::vec2(
		mouse_pos.x - (props_.window_pos.x + props_.window_min.x),
		mouse_pos.y - (props_.window_pos.y + props_.window_min.y)
	);

	// Calculate previous viewport position
	glm::vec2 viewport_pos_prev = viewport_pos - cursor_event.GetOffset();

	// Validate panel size to prevent division by zero
	TY_ASSERT(props_.panel_size.x > TY_EPSILON && props_.panel_size.y > TY_EPSILON, "Invalid viewport panel size");

	// calculate NDC values
	float x_ndc_coord_prev = (2.0f * (viewport_pos_prev.x / props_.panel_size.x)) - 1.0f;
	float y_ndc_coord_prev = (2.0f * (viewport_pos_prev.y / props_.panel_size.y)) - 1.0f;
	if (glm::abs(x_ndc_coord_prev) > 1.0f || glm::abs(y_ndc_coord_prev) > 1.0f) {
		TY_CORE_WARN("Invalid NDC ({:.3f},{:.3f}) ignored. Out of expected range [-1,1]", x_ndc_coord_prev, y_ndc_coord_prev);
		return false;
	}

	float x_ndc_coord = (2.0f * (viewport_pos.x / props_.panel_size.x)) - 1.0f;
	float y_ndc_coord = (2.0f * (viewport_pos.y / props_.panel_size.y)) - 1.0f;
	if (glm::abs(x_ndc_coord) > 1.0f || glm::abs(y_ndc_coord) > 1.0f) {
		TY_CORE_WARN("Invalid NDC ({:.3f},{:.3f}) ignored. Out of expected range [-1,1]", x_ndc_coord, y_ndc_coord);
		return false;
	}

	//TY_INFO("viewport NDC: ({},{})", x_ndc_coord, y_ndc_coord);

	// Only wrap cursor if camera controller is an Orbit camera and the middle mouse botton is pressed
	if (camera_controller_->GetProps().type == ToyEngine::eCameraControllerType::kOrbit && !cursor_event.IsMouseMiddleReleased())
	{
		auto [is_wrapped, new_viewport_pos] = HandleCursorWrapping(x_ndc_coord, y_ndc_coord, viewport_pos);

		if (is_wrapped)
		{
			// Update mouse position
			ImGuiIO& io = ImGui::GetIO();
			io.WantSetMousePos = true;
			io.ConfigNavMoveSetMousePos = true;
			io.MousePos = ImVec2(new_viewport_pos.x + (props_.window_pos.x + props_.window_min.x),
								 new_viewport_pos.y + (props_.window_pos.y + props_.window_min.y));

			//  window_pos is in screen coordinates
			ImGuiViewport* main_viewport = ImGui::GetMainViewport();
			float client_relative_x = (props_.window_pos.x - main_viewport->Pos.x) + props_.window_min.x + new_viewport_pos.x;
			float client_relative_y = (props_.window_pos.y - main_viewport->Pos.y) + props_.window_min.y + new_viewport_pos.y;

			ToyEngine::Window& window = ToyEngine::Application::AccessWindow();
			window.SetCursorPos(client_relative_x, client_relative_y);

			// update ndc values
			float x_ndc_coord = (2.0f * (new_viewport_pos.x / props_.panel_size.x)) - 1.0f;
			float y_ndc_coord = (2.0f * (new_viewport_pos.y / props_.panel_size.y)) - 1.0f;
		}
	}

	// Error handling for large/unexpected jumps in the cursor position, 
	// likely caused by wrapping SetCursorPos
	if (glm::distance(glm::vec2(x_ndc_coord_prev, y_ndc_coord_prev), glm::vec2(x_ndc_coord, y_ndc_coord)) > EXTREME_MOVEMENT_THRESHOLD) {
		TY_CORE_WARN("Ignoring extreme cursor movement - Cursor NDC offset distance: {:.2f}", 
			glm::distance(glm::vec2(x_ndc_coord_prev, y_ndc_coord_prev), glm::vec2(x_ndc_coord, y_ndc_coord)));
		return true;
	}

	cursor_event.SetNDCCoordPrev(glm::vec2(x_ndc_coord_prev, y_ndc_coord_prev));
	cursor_event.SetNDCCoord(glm::vec2(x_ndc_coord, y_ndc_coord)); 

	// Dispatch to camera controller
	ToyEngine::EventDispatcher dispatcher(cursor_event);
	dispatcher.Dispatch<ToyEngine::EventCursorPos>(TY_BINDFN(camera_controller_->OnEvent));

	return cursor_event.GetEventHandled();
}

bool Viewport::OnEvent(ToyEngine::Event& e) {
	ToyEngine::EventDispatcher dispatcher(e);
	dispatcher.Dispatch<ToyEngine::EventVerticalScroll>(TY_BINDFN(camera_controller_->OnEvent));
	dispatcher.Dispatch<ToyEngine::EventCursorPos>(TY_BINDFN(OnMouseMove));

	return e.GetEventHandled();
}

std::pair<bool, glm::vec2> Viewport::HandleCursorWrapping(float x_ndc, float y_ndc, const glm::vec2& viewport_pos)
{
	bool wrapped = false;
	glm::vec2 new_viewport_pos = viewport_pos;

	// Handle horizontal wrapping
	if (x_ndc > CURSOR_WRAP_THRESHOLD) {
		// Cursor hit right edge, wrap to left edge
		wrapped = true;
		new_viewport_pos.x = props_.panel_size.x * (1.0f - CURSOR_WRAP_THRESHOLD);
		TY_TRACE("Cursor wrapped: right to left edge");
	}
	else if (x_ndc < -CURSOR_WRAP_THRESHOLD) {
		// Cursor hit left edge, wrap to right edge
		wrapped = true;
		new_viewport_pos.x = props_.panel_size.x * CURSOR_WRAP_THRESHOLD;
		TY_TRACE("Cursor wrapped: left to right edge");
	}

	// Handle vertical wrapping
	if (y_ndc > CURSOR_WRAP_THRESHOLD) {
		// Cursor hit bottom edge, wrap to top edge
		wrapped = true;
		new_viewport_pos.y = props_.panel_size.y * (1.0f - CURSOR_WRAP_THRESHOLD);
		TY_TRACE("Cursor wrapped: bottom to top edge");
	}
	else if (y_ndc < -CURSOR_WRAP_THRESHOLD) {
		// Cursor hit top edge, wrap to bottom edge
		wrapped = true;
		new_viewport_pos.y = props_.panel_size.y * CURSOR_WRAP_THRESHOLD;
		TY_TRACE("Cursor wrapped: top to bottom edge");
	}
	return std::make_pair(wrapped, new_viewport_pos);
}