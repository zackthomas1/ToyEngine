#pragma once
#include "ToyEngine/enum.h"

#include <glm/vec2.hpp>

namespace ToyEngine
{
  class Event
  {
  public:
    virtual ~Event() {}
    bool GetEventHandled() const { return isHandled_; }
    void SetEventHandled(bool isHandled) { isHandled_ = isHandled; }
  protected:
    Event() : isHandled_(false) {}
  private: 
    bool isHandled_;
  };

  class EventKeyInput : public Event
  {
  public: 
    EventKeyInput(eKeyCode key, eKeyState state) : key_(key), state_(state) {}
    
    eKeyCode GetKeyCode() const { return key_; }
    eKeyState GetKeyState() const { return state_; }

  private: 
    eKeyCode key_;
    eKeyState state_;
  };

  class EventVerticalScroll : public Event
  {
  public:
    EventVerticalScroll(float y_offset) : y_offset_(y_offset) {}
    float GetYOffset() const { return y_offset_; }
  private:
    float y_offset_;
  };

  class EventCursorPos : public Event
  {
  public:
    EventCursorPos(float x_offset, float y_offset,
      float x_ndc_coord_prev, float y_ndc_coord_prev,
      float x_ndc_coord, float y_ndc_coord,
      bool is_mouse_middle_released)
        : offset_(glm::vec2(x_offset, y_offset)),
        ndc_coord_prev(glm::vec2(x_ndc_coord_prev, y_ndc_coord_prev)),
        ndc_coord_(glm::vec2(x_ndc_coord, y_ndc_coord)),
        is_mouse_middle_released_(is_mouse_middle_released)
    {}

    const glm::vec2& GetOffset() const { return offset_; }
    const glm::vec2& GetNDCCoordPrev() const { return ndc_coord_prev; }
    const glm::vec2& GetNDCCoord() const { return ndc_coord_; }
    bool IsMouseMiddleReleased() const { return is_mouse_middle_released_; }

    void SetNDCCoordPrev(glm::vec2& updated_ndc_prev) { ndc_coord_prev = updated_ndc_prev; }
    void SetNDCCoord(glm::vec2& updated_ndc) { ndc_coord_ = updated_ndc; }
  private:
    glm::vec2 offset_, ndc_coord_prev, ndc_coord_;
    bool is_mouse_middle_released_;
  };

  class EventApplicationClose : public Event 
  {
  public:
    EventApplicationClose() {}
  };

  class EventWindowResize : public Event
  {
  public:
    EventWindowResize(unsigned int width, unsigned int height) : width_(width), height_(height) {}
    unsigned int GetWidth() const { return width_; }
    unsigned int GetHeight() const { return height_; }
  private: 
    unsigned int width_, height_;
  };

  class EventDispatcher
  {
  public:
    EventDispatcher(Event& e) : event_(e) {}
    ~EventDispatcher() = default;

    template<typename T, typename F>
    bool Dispatch(const F& func)
    {
      // If the event has already been handled, return true to indicate that it was handled. No further processing is needed.
      if (bool handled = event_.GetEventHandled())
        return handled;

      // Use dynamic_cast to check if the event is of type T. 
      // If it is, call the provided function with the casted event.
      if (T* cast_event = dynamic_cast<T*>(&event_))
      {
        bool is_handled = func(*cast_event);
        event_.SetEventHandled(is_handled);
        return is_handled;
      }
      return false;
    }
  private:
    /* Reference to the event being dispatched */
    Event& event_;
  };
}