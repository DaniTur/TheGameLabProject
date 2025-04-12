#pragma once

#include "GLFW/glfw3.h"

using MouseCode = uint16_t;

namespace MouseBtn {

	enum : MouseCode {

		ButtonLeft = GLFW_MOUSE_BUTTON_LEFT,
		ButtonRight = GLFW_MOUSE_BUTTON_RIGHT
	};
}