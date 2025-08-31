#pragma once
#include "Event.h"

class ApplicationEvent : public Event {
	EventCathegory getEventCathegory() const override {
		return EventCathegory::Application;
	}
};

class WindowResizeEvent : public ApplicationEvent {
public:
	WindowResizeEvent(unsigned int width, unsigned int height)
		: m_Width(width), m_Height(height)
	{
	}

	unsigned int GetWidth() const {
		return m_Width;
	}

	unsigned int GetHeight() const {
		return m_Height;
	}

	EventType getEventType() const override {
		return EventType::WindowResize;
	}

	const char* getName() const override {
		return "WindowResize";
	}

	std::string toString() const override
	{
		std::stringstream ss;
		ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
		return ss.str();
	}

private:
	unsigned int m_Width;
	unsigned int m_Height;
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