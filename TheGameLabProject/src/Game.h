#pragma once

#include "Window.h"
#include "Graphics/Camera.h"
#include "Graphics/ProjectionTransform.h"
#include "Events/Event.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

class Game
{
public:
	
	Game();

	void run();

	void onEvent(Event &event);

private:

	void processInputPolling();
	void onUpdate();

	void onKeyPressed(KeyPressedEvent& e);
	void onKeyReleased(KeyReleasedEvent& e);
	void onMouseMoved(MouseMovedEvent &e);
	void onMouseButtonPressed(MouseButtonPressedEvent &e);
	void onMouseButtonReleased(MouseButtonReleasedEvent &e);

private:

	bool m_running = false;

	unsigned int m_screenWidth = 800;
	unsigned int m_screenHeight = 600;
	Window m_window;

	Camera m_gameCamera;
	// Perspective projection transform
	ProjectionTransform m_projectionTransform;

	double m_DeltaTime{};

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

