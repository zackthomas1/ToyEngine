#pragma once
#include <toy_engine.h>

class Viewport
{
  // viewport variables
  struct ViewportProps {
    bool is_focused, is_hovered;
    ImVec2 panel_size, window_min, window_max, window_pos, window_size;
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
  std::pair<bool, glm::vec2> HandleCursorWrapping(float x_ndc, float y_ndc, const glm::vec2& viewport_pos);
private:
  static constexpr float CURSOR_WRAP_THRESHOLD = 0.96f;
  static constexpr float EXTREME_MOVEMENT_THRESHOLD = 1.0f;
  ViewportProps props_;
  ToyEngine::Ref<ToyEngine::CameraController> camera_controller_;
};
