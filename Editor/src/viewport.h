#pragma once
#include <toy_engine.h>

class Viewport
{
	// viewport variables
	struct ViewportProps {
		bool is_focused, is_hovered;
		ImVec2 panel_size, min, window_pos;
		ToyEngine::Ref<ToyEngine::FrameBuffer> framebuffer;
	};

public:
	Viewport();
	~Viewport() {}

	void ImGuiRender(); 

	bool OnMouseMove(ToyEngine::EventCursorPos& cursor_event); 
	bool OnEvent(ToyEngine::Event& e) ;
	const ViewportProps& GetProps() const { return props_; }
	void SetCameraController(const ToyEngine::Ref <ToyEngine::CameraController> cam) { camera_controller_ = cam; }
private:
	std::pair<bool, glm::vec2> HandleCursorWrapping(float& x_ndc, float& y_ndc, const glm::vec2& viewport_pos);

	void UpdateGLFWCursorPosition(float window_x, float window_y);
private:
	ViewportProps props_;
	ToyEngine::Ref<ToyEngine::CameraController> camera_controller_;
};
