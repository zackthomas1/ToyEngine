#include "viewport.h"
#include "ToyEngine/platform/windows/windows_window.h" // TODO - Remove platform include from viewport 

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
		camera_controller_->OnResize(props_.panel_size.x, props_.panel_size.y);
	}

	ImGui::Image(props_.framebuffer->GetColorAttachment(),
		ImVec2(props_.panel_size.x, props_.panel_size.y),
		ImVec2(0, 1), ImVec2(1, 0));
	ImGui::End();
	ImGui::PopStyleVar();
}

bool Viewport::OnMouseMove(ToyEngine::EventCursorPos& cursor_event)
{
	// check that middle mouse button is pressed
	ToyEngine::InputPoll& input = ToyEngine::Locator::InputPollService();
	if ((input.Mouse(ToyEngine::eMouseCode::kMouseMiddle) == ToyEngine::eKeyState::kRelease))
		return true;
	
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

	// calculate NDC values
	float x_ndc_coord_prev = (2.0f * (viewport_pos_prev.x / props_.panel_size.x)) - 1.0f;
	float y_ndc_coord_prev = (2.0f * (viewport_pos_prev.y / props_.panel_size.y)) - 1.0f;

	float x_ndc_coord = (2.0f * (viewport_pos.x / props_.panel_size.x)) - 1.0f;
	float y_ndc_coord = (2.0f * (viewport_pos.y / props_.panel_size.y)) - 1.0f;
	//TY_INFO("viewport NDC: ({},{})", x_ndc_coord, y_ndc_coord);

	// If cursor was wrapped, HandleCursorWrapping recalculate positions updates x and y ndc values
	auto [wrapped, new_viewport_pos] = HandleCursorWrapping(x_ndc_coord, y_ndc_coord, viewport_pos);
	if (wrapped) {

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
	}

	if (glm::distance(glm::vec2(x_ndc_coord_prev, y_ndc_coord_prev), glm::vec2(x_ndc_coord, y_ndc_coord)) > 1.0f) {
		TY_CORE_WARN("Ignoring extreme cursor movement - Cursor NDC offset distance: {:.2f}", 
			glm::distance(glm::vec2(x_ndc_coord_prev, y_ndc_coord_prev), glm::vec2(x_ndc_coord, y_ndc_coord)));
		return true;
	}

	// Create new event with viewport-relative NDC coordinates
	ToyEngine::EventCursorPos viewport_cursorpos(cursor_event.GetOffset().x, cursor_event.GetOffset().y,
		x_ndc_coord_prev, y_ndc_coord_prev,
		x_ndc_coord, y_ndc_coord,
		wrapped);

	// Dispatch to camera controller
	ToyEngine::EventDispatcher dispatcher(viewport_cursorpos);
	dispatcher.Dispatch<ToyEngine::EventCursorPos>(TY_BINDFN(camera_controller_->OnEvent));

	return true;
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

	const float wrap_threshold = 0.96f; // Wrap when NDC reaches �0.95 

	// Handle horizontal wrapping
	if (x_ndc > wrap_threshold) {
		// Cursor hit right edge, wrap to left edge
		wrapped = true;
		new_viewport_pos.x = props_.panel_size.x * (1.0f - wrap_threshold);
		TY_INFO("Cursor wrapped: right to left edge");
	}
	else if (x_ndc < -wrap_threshold) {
		// Cursor hit left edge, wrap to right edge
		wrapped = true;
		new_viewport_pos.x = props_.panel_size.x * wrap_threshold;
		TY_INFO("Cursor wrapped: left to right edge");
	}

	// Handle vertical wrapping
	if (y_ndc > wrap_threshold) {
		// Cursor hit bottom edge, wrap to top edge
		wrapped = true;
		new_viewport_pos.y = props_.panel_size.y * (1.0f - wrap_threshold);
		TY_INFO("Cursor wrapped: bottom to top edge");
	}
	else if (y_ndc < -wrap_threshold) {
		// Cursor hit top edge, wrap to bottom edge
		wrapped = true;
		new_viewport_pos.y = props_.panel_size.y * wrap_threshold;
		TY_INFO("Cursor wrapped: top to bottom edge");
	}
	return std::make_pair(wrapped, new_viewport_pos);
}