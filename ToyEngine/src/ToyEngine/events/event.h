#pragma once
namespace ToyEngine
{
	enum KeyCode
	{
		kKeyEscape	= 256,
		kKeyW		= 87,
		kKeyS		= 83,
		kKeyA		= 65,
		kKeyD		= 68,
		kKeyE		= 69,
		kKeyQ		= 81,
	};

	enum KeyState
	{
		kRelease	= 0,
		kPress		= 1,
		kHold		= 2,
	};

	class Event
	{
	public:
		Event() : isHandled_(false) {}
		virtual ~Event() {}
		bool GetEventHandled() { return isHandled_; }
		void SetEventHandled(bool isHandled) { isHandled_ = isHandled; }
	private: 
		bool isHandled_;
	};

	class EventKeyInput : public Event
	{
	public: 
		EventKeyInput(KeyCode key, KeyState state) : key_(key), state_(state) {}
		
		KeyCode GetKeyCode() const { return key_; }
		KeyState GetKeyState() const { return state_; }

	private: 
		KeyCode key_; 
		KeyState state_;
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