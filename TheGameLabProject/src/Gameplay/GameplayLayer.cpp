#include "pch.h"
#include "GameplayLayer.h"
//test
#include "KeyCodes.h"
#include "MouseCodes.h"

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
	case Key::DOWN:
		break;
	default:
		break;
	}
	event.handled = true;
	LOG_TRACE("KeyReleased {}", event.getKeyCode());
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
