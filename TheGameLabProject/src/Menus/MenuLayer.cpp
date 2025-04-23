#include "pch.h"
#include "MenuLayer.h"
#include "KeyCodes.h"

void MenuLayer::OnUpdate(double deltaTime)
{
}

void MenuLayer::OnRender()
{
}

void MenuLayer::OnEvent(Event& event)
{
	switch (event.getEventType())
	{
		using enum EventType;	// reduce verbosity
	case KeyPressed:
		onKeyPressed(static_cast<KeyPressedEvent&>(event));
		break;
	default:
		break;
	}
}

void MenuLayer::SetEventCallback(const EventCallback& callback)
{
	m_EventCallback = callback;
}

void MenuLayer::onKeyPressed(KeyPressedEvent& event)
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
	case Key::DOWN:
		break;
	case Key::ESCAPE:
		// TODO: Create a new event for toggle(enable/disable) the menu. Enable/Disable layers, 
		// game should create menu if not existent, or enable/disable it if already created
		//MenuToggleEvent event;
		//m_EventCallback(event);
		//LOG_INFO("Open Menu (GameplayLayer)");
		break;
	default:
		break;
	}
	event.handled = true;
	LOG_TRACE("KeyPressed {}", event.getKeyCode());
}
