#pragma once
#include "ToyEngine/enum/key_enum.h"

namespace ToyEngine
{
	class Event
	{
	public:
		virtual ~Event() {}
		bool GetEventHandled() { return isHandled_; }
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
		EventCursorPos(double xpos, double ypos) : x_offset_(xpos), y_offset_(ypos) {}
		double GetXOffset() { return x_offset_; }
		double GetYOffset() { return y_offset_; }
	private:
		double x_offset_, y_offset_;
	};

	class EventApplicationClose : public Event 
	{
	public:
		EventApplicationClose() {}
	};
}