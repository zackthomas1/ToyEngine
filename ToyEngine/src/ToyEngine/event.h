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
		float GetYOffset() { return y_offset_; }
	private:
		float y_offset_;
	};

	class EventCursorPos : public Event
	{
	public:
		EventCursorPos(float x_offset, float y_offset,
			float x_ndc_coord_prev, float y_ndc_coord_prev,
			float x_ndc_coord, float y_ndc_coord,
			bool is_wrapped = false, glm::vec2 wrap_compensation = glm::vec2(0.0f))
				: offset_(glm::vec2(x_offset, y_offset)), 
				ndc_coord_prev(glm::vec2(x_ndc_coord_prev, y_ndc_coord_prev)),
				ndc_coord_(glm::vec2(x_ndc_coord, y_ndc_coord)),
				is_wrapped_(is_wrapped), wrap_compensation_(wrap_compensation)
		{}
		const glm::vec2& GetOffset() { return offset_; }
		const glm::vec2& GetNDCCoordPrev() { return ndc_coord_prev; }
		const glm::vec2& GetNDCCoord() { return ndc_coord_; }
		
		void SetWrapped(bool wrapped, const glm::vec2& compensation = glm::vec2(0.0f)) {is_wrapped_ = wrapped; wrap_compensation_ = compensation;}
		bool IsWrapped() const { return is_wrapped_;}
		const glm::vec2& GetCompensatedNDC() const { return ndc_coord_ + wrap_compensation_; }
 
	private:
		glm::vec2 offset_, ndc_coord_prev, ndc_coord_;
		bool is_wrapped_;
		glm::vec2 wrap_compensation_;
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
		~EventDispatcher() {}
		
		template<typename T, typename F>
		bool Dispatch(const F& func)
		{
			if (T* cast_event = dynamic_cast<T*>(&event_))
			{
				bool is_handled = func(*cast_event);
				event_.SetEventHandled(is_handled);
				return is_handled;
			}
			return false;
		}
	private:
		Event& event_;
	};
}