#pragma once
#include "Layer.h"
#include "Core/IEventEmitter.h"
#include "Events/KeyEvent.h"

class MenuLayer :
    public Layer, public IEventEmitter
{
public:

    void OnUpdate(double deltaTime) override;
    void OnRender() override;
    void OnEvent(Event& event) override;

    void SetEventCallback(const EventCallback& callback) override;

private:
    void onKeyPressed(KeyPressedEvent& event);
    void onKeyReleased(KeyReleasedEvent& event);

private:
    EventCallback m_EventCallback;
};

