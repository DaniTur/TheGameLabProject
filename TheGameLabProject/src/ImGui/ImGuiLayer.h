#pragma once
#include "Core/Layer.h"
#include "IEventEmitter.h"

#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

#include <Core/Window.h>

class ImGuiLayer : public Layer, public IEventEmitter
{
public:

	explicit ImGuiLayer(Window& window);
	~ImGuiLayer() override;

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

	EventCallback m_EventCallback;

};

