#pragma once
#include "Layer.h"
// test
#include "Events/KeyEvent.h"

class ImGuiLayer : public Layer
{
public:

	ImGuiLayer();

	void OnUpdate(double deltaTime) override;
	void OnRender() override;
	void OnEvent(Event& e) override;

	bool IsActive() const;

private: //test
	void onKeyPressed(KeyPressedEvent& event);
private:

	bool m_Active;
};

