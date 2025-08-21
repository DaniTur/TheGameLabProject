#pragma once
#include "Event.h"

class ApplicationEvent : public Event {
	EventCathegory getEventCathegory() const override {
		return EventCathegory::Application;
	}
};

class WindowClosedEvent : public ApplicationEvent {
public:
	WindowClosedEvent() = default;

	EventType getEventType() const override {
		return EventType::WindowClosed;
	}

	const char* getName() const override {
		return "WindowClosed";
	}
};

class ToggleLayerEvent : public ApplicationEvent {
public:
	explicit ToggleLayerEvent(const std::string &name) 
		: m_layerId(name)
	{
	}

	EventType getEventType() const override {
		return EventType::ToggleLayer;
	}

	const char* getName() const override {
		return "ToggleLayer";
	}

	const std::string& getLayerId() const {
		return m_layerId;
	}
private:
	std::string m_layerId = "";
};