#pragma once
#include "Core/Layer.h"
// test
#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

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

