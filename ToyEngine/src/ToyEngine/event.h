#pragma once
namespace ToyEngine
{
	enum KeyCode
	{
		key_escape = 256, 
		key_w = 87, 
		key_s = 83,
	};

	class Event
	{
	public:
		virtual ~Event() {}
	};

	class EventKeyPress: public Event
	{
	public:
		EventKeyPress(KeyCode key) : key_(key) {}
		KeyCode GetKeyCode() const { return key_; }
	private:
		KeyCode key_;
	};

	class EventKeyRelease : public Event
	{
	public:
		EventKeyRelease(KeyCode key) : key_(key) {}
		KeyCode GetKeyCode() const { return key_; }
	private:
		KeyCode key_;
	};
}