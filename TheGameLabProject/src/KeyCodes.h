#pragma once

#include "GLFW/glfw3.h"

using KeyCode = uint16_t;

namespace Key {

	enum : KeyCode {

		W = GLFW_KEY_W,
		A = GLFW_KEY_A,
		S = GLFW_KEY_S,
		D = GLFW_KEY_D,
		Space = GLFW_KEY_SPACE,
		UP = GLFW_KEY_UP,
		LEFT = GLFW_KEY_LEFT,
		DOWN = GLFW_KEY_DOWN,
		RIGHT = GLFW_KEY_RIGHT
	};

}