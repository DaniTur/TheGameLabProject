#include "pch.h"

#include "Window.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

Window::Window(unsigned int width, unsigned int height)
	: m_cameraTarget(0.0f)
{
	m_windowData.width = width;
	m_windowData.height = height;
	
	if (glfwInit() == GLFW_FALSE) {
		std::string errorMessage = "Could not initialize GLFW";
		LOG_FATAL("[Window] {}", errorMessage);
		throw std::exception(errorMessage.c_str());
	}

	// Using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Using Core subset of features without the backwards-compatible features
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	LOG_INFO("[Window] GLFW initialized");

	m_window = glfwCreateWindow(m_windowData.width, m_windowData.height, "TheGameLabProject", nullptr, nullptr);
	if (m_window == nullptr) {
		LOG_FATAL("[Window] Failed to create GLFW window");
		glfwTerminate();
	}

	glfwMakeContextCurrent(m_window);
	setVSync(true);
	centerWindow();

	// Stores the Window instance inside the GLFW structure to retrieve it inside the glfw callbacks, this way
	// we have acces to the Windows instance inside the glfw callbacks
	glfwSetWindowUserPointer(m_window, &m_windowData);
	
	// Window Close
	glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window) {
			const WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowClosedEvent event;
			data.eventCallback(event);
		});

	// Window Resize callback
	glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
		// Glfw automaticaly calls glfwSetWindowSize(w, h) to set the new glfw window size
		const WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
		WindowResizeEvent event(width, height);
		data.eventCallback(event);
		});

	// Key Inputs on Window callback
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

	// Key Input as printable characters callback
	glfwSetCharCallback(m_window, [](GLFWwindow* window, unsigned int keycode) {
			const WindowData& windowData = *(WindowData*)(glfwGetWindowUserPointer(window));
			KeyTypedEvent event(keycode);
			windowData.eventCallback(event);
		});

	// Mouse settings
	setMouseCursorCapture(true);
	setMouseAcceleration(false);
	setMouseInputsCallbacks();

	// TODO: Move this away to another class like Renderer, this doesnt belong to Window
	// GLFW tiene las funciones de OpenGL necesarias segun el sistema operativo y el contexto activo, 
	// se obtienen dichas funciones mediante una direccion y le decimos a GLAD que cargue esas funciones en memoria desde los drivers
	// GLFW knows where the OpenGL functions are located inside the operative system(OpenGL 1.1 functions for Windows) 
	// and in the driver list of functions(above OpenGL 1.1 version). GLFW gives the address of the function list (OS + drivers)
	// to GLAD to be able to load those functions.
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::string errorMessage = "Failed to initialize GLAD";
		LOG_FATAL("[Window] {}", errorMessage);
		throw std::exception(errorMessage.c_str());
	}
}

Window::~Window() {
	close();
}

void Window::setEventCallback(const EventCallbackFn& callback) { 
	m_windowData.eventCallback = callback; 
}

void Window::setVSync(bool enable) const {
	glfwSwapInterval(enable);
}

bool Window::shouldClose()
{
	return glfwWindowShouldClose(m_window);
}

// Release all previously allocated GLFW resources.
void Window::close() {
	glfwSetWindowUserPointer(m_window, nullptr); // Is not strictly necesary for the actual use case
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

void Window::onUpdate()
{
	glfwPollEvents(); // this funtion executes our callbacks setted for the window events
	glfwSwapBuffers(m_window);
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

unsigned int Window::GetWidth() const
{
	return m_windowData.width;
}

unsigned int Window::GetHeight() const
{
	return m_windowData.height;
}

// Center the window(assuming a 1920x1080 monitor resolution)
void Window::centerWindow() {
	glfwSetWindowPos(m_window, (1920 / 2) - (m_windowData.width/ 2), (1080 / 2) - (m_windowData.height / 2));
}

void Window::setMouseCursorCapture(bool capture)
{
	if (capture) {
		// Enable mouse movement inputs capture and the window will capture the mouse cursor
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	} else {
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

void Window::setMouseAcceleration(bool active) {
	// Raw motion mode ignores mouse acceleration and scaling
	if (active) {
		glfwSetInputMode(m_window, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
	} else {
		glfwSetInputMode(m_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	}
}

void Window::setNewSize(unsigned int width, unsigned int height)
{
	m_windowData.width = width;
	m_windowData.height = height;
	//glfwSetWindowSize(m_window, m_windowData.width, m_windowData.height);
	//glViewport(0, 0, m_windowData.width, m_windowData.height);
}

void Window::setMousePosition(float xpos, float ypos)
{
	glfwSetCursorPos(m_window, xpos, ypos);
}

void Window::setMouseInputsCallbacks() {
	
	// Mouse position/moved callback
	glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xpos, double ypos) {
			const WindowData &windowData = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseMovedEvent event((float)xpos, (float)ypos);
			windowData.eventCallback(event);
		});

	// Mouse buttons callback
	glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods) {
			const WindowData& windowData = *(WindowData*)glfwGetWindowUserPointer(window);
			if (action == GLFW_PRESS) {
				MouseButtonPressedEvent event(button);
				windowData.eventCallback(event);
			} else if (action == GLFW_RELEASE) {
				MouseButtonReleasedEvent event(button);
				windowData.eventCallback(event);
			}
		});

	// Mouse Wheel callback
	glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xoffset, double yoffset) {
			const WindowData& windowData = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseScrolledEvent event((float)xoffset, (float)yoffset);
			windowData.eventCallback(event);
		});
}
