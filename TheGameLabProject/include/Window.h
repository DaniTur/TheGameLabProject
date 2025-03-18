#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Camera.h>
#include <unordered_map>

class Window {
public:

	Window() = default;
	Window(unsigned int width, unsigned int height);
	~Window();

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
	void handleMouseMovement(double xpos, double ypos);
	void handleMouseButtonInput(int button, int action, int mods);

private:

	GLFWwindow* m_window;

	unsigned int m_width;
	unsigned int m_height;

	// Mouse
	double m_lastX = 400.0;
	double m_lastY = 300.0;
	float m_yaw = -90.0f;
	float m_pitch = 0.0f;
	bool m_firstMouse = true;
	bool m_zoomEnable = false;

	glm::vec3 m_cameraTarget;
	std::unordered_map<int, int> m_mouseButtonState;
};

