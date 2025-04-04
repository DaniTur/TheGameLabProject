#pragma once

#include "glm.hpp"
#include "KeyCodes.h"
#include "GLFW/glfw3.h"

// Make this class an interface and implement it per platform if needed

class Input {
public:

	static void Init(GLFWwindow* window);

	static bool IsKeyPressed(KeyCode key);
	static bool IsMouseButtonPressed(int button);
	static glm::vec2 GetMousePosition();

private:
	static GLFWwindow* s_Window;
};