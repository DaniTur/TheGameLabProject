#pragma once
#include "Layer.h"
//test
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
#include "IEventEmitter.h"

class GameplayLayer : public Layer, public IEventEmitter
{
public:
	GameplayLayer();

	void OnUpdate(double deltaTime) override;
	void OnRender() override;
	void OnEvent(Event& e) override;

	bool IsActive() const;

	void SetEventCallback(const EventCallback& callback) override;

private:
	void onKeyPressed(KeyPressedEvent& event);
	void onKeyReleased(KeyReleasedEvent& event);
	void onMouseMoved(MouseMovedEvent& event);
	void onMouseButtonPressed(MouseButtonPressedEvent& event);
	void onMouseButtonReleased(MouseButtonReleasedEvent& event);

private:

	bool m_Active;

	EventCallback m_EventCallback;
};

