#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Camera.h>

class Window {
public:

	Window() = default;
	Window(unsigned int width, unsigned int height);
	~Window();

	void disableVSync() const;

	bool shouldClose();

	void close();

	void swapBuffers();
	void pollEvents(); // user events

	GLFWwindow* get();

private:

	void centerWindow();
	void onWindowResize();

private:

	GLFWwindow* m_window;

	unsigned int m_width;
	unsigned int m_height;
};

