#pragma once
#include "Event.h"

class KeyEvent : public Event {
public:

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
};