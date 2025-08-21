#pragma once
#include "Core/Layer.h"
#include "IEventEmitter.h"

#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

class ImGuiLayer : public Layer, public IEventEmitter
{
public:

	ImGuiLayer();

	void OnUpdate(double deltaTime) override;
	void OnRender() override;
	void OnEvent(Event& e) override;

	bool IsActive() const;

	void SetEventCallback(const EventCallback& callback) override;

private:

	EventCallback m_EventCallback;

	void onKeyPressed(KeyPressedEvent& event);

};

