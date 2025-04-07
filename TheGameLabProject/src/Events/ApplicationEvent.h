#pragma once
#include "Event.h"

class WindowClosedEvent : public Event {
public:
	WindowClosedEvent() = default;

	EventType getEventType() const override {
		return EventType::WindowClosed;
	}

	const char* getName() const override {
		return "WindowClosed";
	}
};