#pragma once
#include "ToyEngine/enum.h"

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
		EventCursorPos(double xpos, double ypos) : x_offset_(xpos), y_offset_(ypos) {}
		double GetXOffset() const { return x_offset_; }
		double GetYOffset() const { return y_offset_; }
	private:
		double x_offset_, y_offset_;
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
}