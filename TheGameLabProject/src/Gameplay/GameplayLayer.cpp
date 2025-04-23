#include "pch.h"
#include "GameplayLayer.h"
#include "KeyCodes.h"
#include "MouseCodes.h"
#include "Events/ApplicationEvent.h"
#include "glm.hpp"

GameplayLayer::GameplayLayer() 
	: Layer("GameplayLayer"), m_Active(true)
{
}

void GameplayLayer::OnUpdate(double deltaTime)
{
	LOG_TRACE("Gameplay Update");
}

void GameplayLayer::OnRender()
{
}

// GameplayLayer is always the bottom layer, so all the events that reach this layer should be always handled
void GameplayLayer::OnEvent(Event& event)
{
	switch (event.getEventType())
	{
	using enum EventType;	// reduce verbosity
	case KeyPressed:
		onKeyPressed(static_cast<KeyPressedEvent&>(event));
		break;
	case MouseMoved:
		onMouseMoved();
		break;
	default:
		break;
	}
}

bool GameplayLayer::IsActive() const
{
	return m_Active;
}

void GameplayLayer::onKeyPressed(KeyPressedEvent& event)
{
	switch (event.getKeyCode())
	{
	case Key::W:
		break;
	case Key::A:
		break;
	case Key::S:
		break;
	case Key::D:
		break;
	case Key::UP:
		break;
	case Key::F1:
		break;
	case Key::ESCAPE:
	{
		// TODO: Create a new event for toggle(enable/disable) the menu. Enable/Disable layers, 
		// game should create menu if not existent, or enable/disable it if already created
		//MenuToggleEvent event;
		//m_EventCallback(event);
		WindowClosedEvent windowCloseEvent;
		m_EventCallback(windowCloseEvent);
		break;
	}
	default:
		break;
	}
	event.handled = true;
	LOG_TRACE("KeyPressed {}", event.getKeyCode());
}

void GameplayLayer::onKeyReleased(KeyReleasedEvent& e)
{
	switch (e.getKeyCode())
	{
	case Key::W:
		break;
	case Key::A:
		break;
	case Key::S:
		break;
	case Key::D:
		break;
	case Key::UP:
		break;
	case Key::DOWN:
		break;
	default:
		break;
	}
	e.handled = true;
	LOG_TRACE("KeyReleased {}", e.getKeyCode());
}

void GameplayLayer::onMouseMoved(MouseMovedEvent& event)
{
	if (m_Mouse.firstMouse)
	{
		m_Mouse.lastX = e.getX();
		m_Mouse.lastY = e.getY();
		m_Mouse.firstMouse = false;
	}

	double xoffset = e.getX() - m_Mouse.lastX;
	double yoffset = m_Mouse.lastY - e.getY();
	m_Mouse.lastX = e.getX();
	m_Mouse.lastY = e.getY();

	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	// The mouse movement offset is equal to the angle of the camera we will move
	m_Mouse.yaw += (float)xoffset;
	m_Mouse.pitch += (float)yoffset;

	// Pitch limitation
	if (m_Mouse.pitch > 89.0f)
		m_Mouse.pitch = 89.0f;
	if (m_Mouse.pitch < -89.0f)
		m_Mouse.pitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(m_Mouse.yaw)) * cos(glm::radians(m_Mouse.pitch));
	direction.y = sin(glm::radians(m_Mouse.pitch));
	direction.z = sin(glm::radians(m_Mouse.yaw)) * cos(glm::radians(m_Mouse.pitch));
	m_gameCamera.setCameraTarget(glm::normalize(direction));

	e.handled = true;
	//LOG_TRACE("MouseMoved x:{} y:{}", e.getX(), e.getY());
}

void GameplayLayer::onMouseButtonPressed(MouseButtonPressedEvent& event)
{
	switch (event.getButtonCode())
	{
	case MouseBtn::ButtonLeft:
		break;
	case MouseBtn::ButtonRight:
		break;
	default:
		break;
	}
	event.handled = true;
	LOG_TRACE("ButtonPressed {}", event.getButtonCode());
}

void GameplayLayer::onMouseButtonReleased(MouseButtonReleasedEvent& event)
{
	switch (event.getButtonCode())
	{
	case MouseBtn::ButtonLeft:
		break;
	case MouseBtn::ButtonRight:
		break;
	default:
		break;
	}
	event.handled = true;
	LOG_TRACE("ButtonReleased {}", event.getButtonCode());
}

void GameplayLayer::SetEventCallback(const EventCallback& callback)
{
	m_EventCallback = callback;
}
