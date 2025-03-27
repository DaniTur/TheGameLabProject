#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <unordered_map>
#include <functional>
#include "Graphics/Camera.h"
#include "Events/KeyEvent.h"

using EventCallbackFn = std::function<void(Event&)>;

class Window {
public:

	Window() = default;
	Window(unsigned int width, unsigned int height);
	~Window();

	void setEventCallback(const EventCallbackFn& callback);

	void setVSync(bool enable) const;

	bool shouldClose();

	void close();

	void swapBuffers();
	void pollEvents() const; // user events

	glm::vec3 getCameraTarget() const;
	int getMouseButtonState(int button);

	GLFWwindow* get();

private:

	void centerWindow();
	void onWindowResize();
	
	// Mouse input
	void setMouseInputsCallbacks();
	static void mouseMovementInputCallback(GLFWwindow* window, double xpos, double ypos);
	static void mouseButtonInputCallback(GLFWwindow* window, int button, int action, int mods);
	void handleMouseButtonInput(int button, int action, int mods);

private:

	GLFWwindow* m_window;

	struct WindowData {

		std::string title;
		unsigned int width{};
		unsigned int height{};
		// Function used when the window executes event callbacks(glfw callbacks)
		EventCallbackFn eventCallback;
	};

	WindowData m_windowData;

	glm::vec3 m_cameraTarget;
	std::unordered_map<int, int> m_mouseButtonState;
};

