#pragma once
#include "Layer.h"
//test
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

class GameplayLayer : public Layer
{
public:
	GameplayLayer();

	void OnUpdate(double deltaTime) override;
	void OnRender() override;
	void OnEvent(Event& e) override;

	bool IsActive() const;

private:
	void onKeyPressed(KeyPressedEvent& event);
	void onKeyReleased(KeyReleasedEvent& event);
	void onMouseButtonPressed(MouseButtonPressedEvent& event);
	void onMouseButtonReleased(MouseButtonReleasedEvent& event);

private:

	bool m_Active;
};

