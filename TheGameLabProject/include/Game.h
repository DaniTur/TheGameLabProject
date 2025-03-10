#pragma once

#include <Window.h>
#include <Camera.h>
#include <ProjectionTransform.h>

class Game
{
public:
	
	Game();

	void run();

private:

	void processInput(double deltaTime);

private:

	bool m_running = false;

	unsigned int m_screenWidth = 800;
	unsigned int m_screenHeight = 600;
	Window m_window;

	Camera m_gameCamera;
	// Perspective projection transform
	ProjectionTransform projectionTransform;
};

