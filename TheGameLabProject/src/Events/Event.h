#pragma once

#include <string>

enum class EventType {
	None = 0,
	KeyPressed, KeyReleased, KeyTyped,
	MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled,
	WindowClosed, WindowResize,
	ToggleLayer
};

enum class EventCathegory {
	Application,
	Layer
};

// Abstract Class
class Event {
public:
	virtual ~Event() = default;

	virtual EventCathegory getEventCathegory() const = 0;
	virtual EventType getEventType() const = 0;
	virtual const char* getName() const = 0;
	virtual std::string toString() const { return getName(); }

	bool handled = false;

	// hidden friend para ADL
	friend std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.toString();
	}
};
