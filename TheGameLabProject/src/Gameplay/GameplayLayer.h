#pragma once
#include "Layer.h"
#include "IEventEmitter.h"

#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
// rendering
#include "Graphics/shader.h"
#include "Graphics/Model.h"
#include "Graphics/WorldTransform.h"
#include "Graphics/ProjectionTransform.h"
#include "Graphics/Camera.h"

#include <Scene/Scene.h>

class GameplayLayer : public Layer, public IEventEmitter
{
public:
	GameplayLayer();

	void OnUpdate(double deltaTime) override;
	void OnRender() override;
	void OnEvent(Event& e) override;

	void SetEventCallback(const EventCallback& callback) override;

	Scene& GetActiveScene();

private:
	void processInputPolling(double deltaTime);
	void onKeyPressed(KeyPressedEvent& event);
	void onKeyReleased(KeyReleasedEvent& event);
	void onMouseMoved(MouseMovedEvent& event);
	void onMouseButtonPressed(MouseButtonPressedEvent& event);
	void onMouseButtonReleased(MouseButtonReleasedEvent& event);

private:

	EventCallback m_EventCallback;

	// Owns the virtual mouse data
	struct MouseState {
		float sensitivity = 0.1f;
		float yaw = -90.0f;
		float pitch = 0.0f;
		bool zoomEnable = false;
	};;
	MouseState m_MouseState;

	Scene m_ActiveScene;	// default Scene
	Camera m_gameCamera;
};

