#pragma once

#include <Window.h>
#include <Camera.h>
#include <ProjectionTransform.h>
#include "Event.h"

class Game
{
public:
	
	Game();

	void run();

	void onEvent(Event &event);

private:

	void processInput(double deltaTime);

private:

	bool m_running = false;

	unsigned int m_screenWidth = 800;
	unsigned int m_screenHeight = 600;
	Window m_window;

	Camera m_gameCamera;
	// Perspective projection transform
	ProjectionTransform m_projectionTransform;

	bool m_PlayerWalking = false;
};

