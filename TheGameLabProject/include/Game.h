#pragma once

#include <Window.h>
#include <Camera.h>
#include <ProjectionTransform.h>
#include "Event.h"
#include "KeyEvent.h"
#include "MouseEvent.h"

class Game
{
public:
	
	Game();

	void run();

	void onEvent(Event &event);

private:

	void processInput(double deltaTime);

	void onKeyPressed(KeyPressedEvent& e);
	void onKeyReleased(KeyReleasedEvent& e);
	void onMouseMoved(MouseMovedEvent &e);

private:

	bool m_running = false;

	unsigned int m_screenWidth = 800;
	unsigned int m_screenHeight = 600;
	Window m_window;

	Camera m_gameCamera;
	// Perspective projection transform
	ProjectionTransform m_projectionTransform;

	double m_DeltaTime{};
	bool m_PlayerWalking = false;

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

