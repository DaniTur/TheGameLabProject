#pragma once
#include "Event.h"

class KeyEvent : public Event {
public:

	EventCathegory getEventCathegory() const override{
		return EventCathegory::Layer;
	}

	int getKeyCode() const {
		return m_KeyCode;
	}

protected:
	
	KeyEvent(const int keycode) 
		: m_KeyCode(keycode) {}
	
	int m_KeyCode;
};


class KeyPressedEvent : public KeyEvent {
public:
	
	explicit KeyPressedEvent(const int keycode) : KeyEvent(keycode) {}

	EventType getEventType() const override {
		return EventType::KeyPressed;
	}

	const char* getName() const override {
		return "KeyPressed";
	}
};

class KeyReleasedEvent : public KeyEvent {
public:

	explicit KeyReleasedEvent(const int keycode) : KeyEvent(keycode) {}

	EventType getEventType() const override {
		return EventType::KeyReleased;
	}

	const char* getName() const override {
		return "KeyReleased";
	}

	std::string toString() const override {
		std::stringstream ss;
		ss << "KeyReleasedEvent: " << m_KeyCode;
		return ss.str();
	}
};

class KeyTypedEvent : public KeyEvent {
public:

	explicit KeyTypedEvent(int keycode) : KeyEvent(keycode) {}

	EventType getEventType() const override {
		return EventType::KeyTyped;
	}

	const char* getName() const override {
		return "KeyTyped";
	}

	std::string toString() const override {
		std::stringstream ss;
		ss << "KeyTypedEvent: " << m_KeyCode;
		return ss.str();
	}
};