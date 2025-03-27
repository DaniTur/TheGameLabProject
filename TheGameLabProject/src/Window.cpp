#include <iostream>

#include "Logging/Logger.h"
#include "Window.h"
#include "Events/MouseEvent.h"

Window::Window(unsigned int width, unsigned int height)
	: m_cameraTarget(0.0f)
{
	m_windowData.width = width;
	m_windowData.height = height;
	
	if (glfwInit() == GLFW_FALSE) {
		std::string errorMessage = "Could not initialize GLFW";
		LOG_ERROR(errorMessage);
		throw std::exception(errorMessage.c_str());
	}

	// Using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Using Core subset of features without the backwards-compatible features
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	LOG_INFO("GLFW initialized");

	m_window = glfwCreateWindow(m_windowData.width, m_windowData.height, "TheGameLabProject", nullptr, nullptr);
	if (m_window == nullptr) {
		LOG_FATAL("Failed to create GLFW window");
		glfwTerminate();
	}

	glfwMakeContextCurrent(m_window);
	centerWindow();
	onWindowResize();
	setVSync(false);
	// Stores the Window instance inside the GLFW structure to retrieve it inside the glfw callbacks, this way
	// we have acces to the Windows instance inside the glfw callbacks
	glfwSetWindowUserPointer(m_window, &m_windowData);
	
	glfwSetKeyCallback(m_window, [](GLFWwindow* window,int key, int scancode, int action, int mods) {
		const WindowData& windowData = *(WindowData*)(glfwGetWindowUserPointer(window));
			if (action == GLFW_PRESS) {
				KeyPressedEvent event(key);
				windowData.eventCallback(event);
			}
			else if (action == GLFW_RELEASE) {
				KeyReleasedEvent event(key);
				windowData.eventCallback(event);
			}
		});
	setMouseInputsCallbacks();

	// TODO: Move this away to another class like Renderer, this doesnt belong to Window
	// GLFW tiene las funciones de OpenGL necesarias segun el sistema operativo y el contexto activo, 
	// se obtienen dichas funciones mediante una direccion y le decimos a GLAD que cargue esas funciones en memoria desde los drivers
	// GLFW knows where the OpenGL functions are located inside the operative system(OpenGL 1.1 functions for Windows) 
	// and in the driver list of functions(above OpenGL 1.1 version). GLFW gives the address of the function list (OS + drivers)
	// to GLAD to be able to load those functions.
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::string errorMessage = "Failed to initialize GLAD";
		LOG_FATAL(errorMessage);
		throw std::exception(errorMessage.c_str());
	}
}

Window::~Window() {
	glfwSetWindowUserPointer(m_window, nullptr); // Is not strictly necesary for the actuall case uses
	glfwDestroyWindow(m_window);
	close();
}

void Window::setEventCallback(const EventCallbackFn& callback) { 
	m_windowData.eventCallback = callback; 
}

void Window::setVSync(bool enable) const {
	if (!enable) {
		glfwSwapInterval(0);
	}
}

bool Window::shouldClose()
{
	return glfwWindowShouldClose(m_window);
}

// Release all previously allocated GLFW resources.
void Window::close() {
	glfwTerminate();
}

void Window::swapBuffers()
{
	glfwSwapBuffers(m_window);
}

void Window::pollEvents() const
{
	glfwPollEvents();
}

glm::vec3 Window::getCameraTarget() const
{
	return m_cameraTarget;
}

int Window::getMouseButtonState(int button) {
	// The specified key doesn't exists in the mouseButtonState map
	if (m_mouseButtonState.count(button) <= 0) {
		return -1;
	}
	return m_mouseButtonState.at(button);
}

GLFWwindow* Window::get()
{
	return m_window;
}

// Center the window(assuming a 1920x1080 monitor resolution)
void Window::centerWindow() {
	glfwSetWindowPos(m_window, (1920 / 2) - (m_windowData.width/ 2), (1080 / 2) - (m_windowData.height / 2));
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	// Tells OpenGL the size of the render area inside the window. Indica a OpenGL el area dentro de la ventana donde va a tener que renderizar
	glViewport(0, 0, width, height);
}

void Window::onWindowResize() {
	// On window resize, call the parameter function
	glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
}

void Window::setMouseInputsCallbacks() {
	// Enable mouse movement inputs capture and the window will capture the mouse cursor
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// Raw motion mode ignores mouse acceleration and scaling
	if (glfwRawMouseMotionSupported()) {
		glfwSetInputMode(m_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	}
	// Fuction call back for mouse inputs
	glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xpos, double ypos) {
			const WindowData &windowData = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseMovedEvent event((float)xpos, (float)ypos);
			windowData.eventCallback(event);
		});
	// Enable mouse button inputs
	glfwSetMouseButtonCallback(m_window, Window::mouseButtonInputCallback);
}


void Window::mouseButtonInputCallback(GLFWwindow* window, int button, int action, int mods) {
	Window* thisWindowInstance = static_cast<Window*>(glfwGetWindowUserPointer(window));
	if (thisWindowInstance) {
		thisWindowInstance->handleMouseButtonInput(button, action, mods);
	}
}


// TODO: Handle mouse buttons release
void Window::handleMouseButtonInput(int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		m_mouseButtonState.insert_or_assign(GLFW_MOUSE_BUTTON_LEFT, GLFW_PRESS);
	} else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		m_mouseButtonState.insert_or_assign(GLFW_MOUSE_BUTTON_LEFT, GLFW_RELEASE);
	}
	//if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
//	if (!zoomEnable) {
//		zoomEnable = true;
//		projectionTransform.setFOV(120.0f);
//	}
//	else {
//		zoomEnable = false;
//		projectionTransform.setFOV(90.0f);
//	}
//}
}
