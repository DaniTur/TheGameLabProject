#pragma once

#include <string>

enum class EventType {
	None = 0,
	KeyPressed, KeyReleased,
	MouseButtonPressed, MouseButtonRelased, MouseMoved, MouseScrolled
};

// Abstract Class
class Event {
public:
	virtual ~Event() = default;

	virtual EventType getEventType() const = 0;
	virtual const char* getName() const = 0;
	virtual std::string toString() { return getName(); }

	bool handled = false;
};