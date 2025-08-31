#pragma once

#include "Window.h"
#include "Graphics/Camera.h"
#include "Graphics/ProjectionTransform.h"
#include "Events/Event.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
#include "Events/ApplicationEvent.h"
#include "Core/LayerStack.h"

class Game
{
public:
	
	Game();

	void run();

	void onEvent(Event &event);

private:

	void processInputPolling();
	void onUpdate();

	void onWindowClosed(WindowClosedEvent& e);
	void onWindowResize(WindowResizeEvent& event);
	void onKeyPressed(KeyPressedEvent& e);
	void onKeyReleased(KeyReleasedEvent& e);
	void onMouseMoved(MouseMovedEvent &e);
	void onMouseButtonPressed(MouseButtonPressedEvent &e);
	void onMouseButtonReleased(MouseButtonReleasedEvent &e);
	void onToggleLayer(ToggleLayerEvent& e);

private:

	bool m_running = false;

	double m_DeltaTime{};

	LayerStack m_LayerStack;

	unsigned int m_screenWidth = 1280;
	unsigned int m_screenHeight = 720;
	Window m_window;

	Camera m_gameCamera;
	// Perspective projection transform
	ProjectionTransform m_projectionTransform;


	struct Mouse {
		double lastX = 400.0;
		double lastY = 300.0;
		float yaw = -90.0f;
		float pitch = 0.0f;
		bool firstMouse = true;
		bool zoomEnable = false;
	};
	Mouse m_Mouse;
};

