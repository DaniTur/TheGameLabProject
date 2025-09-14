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

	//void processInputPolling();
	void onUpdate();

	void onWindowClosed(WindowClosedEvent& event);
	void onWindowResize(WindowResizeEvent& event);
	void onKeyPressed(KeyPressedEvent& event);
	void onKeyReleased(KeyReleasedEvent& event);
	void onMouseMoved(MouseMovedEvent& event);
	void onMouseButtonPressed(MouseButtonPressedEvent& event);
	void onMouseButtonReleased(MouseButtonReleasedEvent& event);
	void onToggleLayer(ToggleLayerEvent& event);

	// Resets the mouse position to the center of the window, not the screen
	void resetMouseToCenterWindow();

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

	// Owns the physical mouse (manages the OS mouse movement callbacks)
	struct Mouse {
		float lastX = 400.0;
		float lastY = 300.0;
		bool firstMouse = true;
	};
	Mouse m_Mouse;
};

