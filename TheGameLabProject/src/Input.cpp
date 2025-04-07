#include "pch.h"
#include "Input.h"

GLFWwindow* Input::s_Window = nullptr;

void Input::Init(GLFWwindow* window)
{
	s_Window = window;
}

bool Input::IsKeyPressed(KeyCode key)
{
	bool state = glfwGetKey(s_Window, key);
	return state == GLFW_PRESS;
}

bool Input::IsMouseButtonPressed(MouseCode button)
{
	bool state = glfwGetMouseButton(s_Window, button);
	return state == GLFW_PRESS;
}

glm::vec2 Input::GetMousePosition()
{
	double xpos;
	double ypos;
	glfwGetCursorPos(s_Window, &xpos, &ypos);
	return { (float)xpos, (float)ypos };
}
