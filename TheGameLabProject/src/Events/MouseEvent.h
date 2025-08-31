#pragma once

#include "Event.h"
#include <sstream>

class MouseMovedEvent : public Event {
public:
	MouseMovedEvent(float xpos, float ypos) 
		: m_MouseX(xpos), m_MouseY(ypos) 
	{
	}

	float getX() const {
		return m_MouseX;
	}

	float getY() const {
		return m_MouseY;
	}

	EventCathegory getEventCathegory() const override{
		return EventCathegory::Layer;
	}

	EventType getEventType() const override {
		return EventType::MouseMoved;
	}

	const char* getName() const override {
		return "MouseMoved";
	}

	std::string toString() const override {
		std::stringstream ss;
		ss << "MouseMovedEvent: " << m_MouseX << ", " << m_MouseY;
		return ss.str();
	}

private:
	float m_MouseX;
	float m_MouseY;
};

class MouseScrolledEvent : public Event {
public:
	MouseScrolledEvent(float xOffset, float yOffset) :
		m_XOffset(xOffset), m_YOffset(yOffset)
	{
	}

	float GetXOffset() const {
		return m_XOffset;
	}
	
	float GetYOffset() const {
		return m_YOffset;
	}

	EventCathegory getEventCathegory() const override {
		return EventCathegory::Layer;
	}

	EventType getEventType() const override {
		return EventType::MouseScrolled;
	}

	const char* getName() const override {
		return "MouseScrolled";
	}

	std::string toString() const override {
		std::stringstream ss;
		ss << "MouseScrolledEvent: " << m_XOffset << ", " << m_YOffset;
		return ss.str();
	}

private:
	float m_XOffset;
	float m_YOffset;
};

class MouseButtonEvent : public Event {
public:

	EventCathegory getEventCathegory() const override{
		return EventCathegory::Layer;
	}

	int getButtonCode() const {
		return m_ButtonCode;
	}

protected:
	
	MouseButtonEvent(const int buttoncode) 
		: m_ButtonCode(buttoncode) {}
	
	int m_ButtonCode;
};


class MouseButtonPressedEvent : public MouseButtonEvent {
public:

	explicit MouseButtonPressedEvent(const int buttoncode) : MouseButtonEvent(buttoncode) {}

	EventType getEventType() const override {
		return EventType::MouseButtonPressed;
	}

	const char* getName() const override {
		return "MouseButtonPressed";
	}

	std::string toString() const override {
		std::stringstream ss;
		ss << "MouseButtonPressed: " << m_ButtonCode;
		return ss.str();
	}
};


class MouseButtonReleasedEvent : public MouseButtonEvent {
public:

	explicit MouseButtonReleasedEvent(const int buttoncode) : MouseButtonEvent(buttoncode) {}

	EventType getEventType() const override {
		return EventType::MouseButtonReleased;
	}

	const char* getName() const override {
		return "MouseButtonReleased";
	}

	std::string toString() const override {
		std::stringstream ss;
		ss << "MoueButtonReleasedEvent: " << m_ButtonCode;
		return ss.str();
	}
};