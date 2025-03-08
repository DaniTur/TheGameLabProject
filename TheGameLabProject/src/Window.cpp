#include "Window.h"
#include <iostream>

Window::Window(unsigned int width, unsigned int height)
	: m_width(width), m_height(height)
{
	glfwInit();

	// Using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Using Core subset of features without the backwards-compatible features
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_window = glfwCreateWindow(m_width, m_height, "TheGameLabProject", nullptr, nullptr);
	if (m_window == nullptr) {

		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}

	glfwMakeContextCurrent(m_window);

	// GLFW tiene las funciones de OpenGL necesarias según el sistema operativo y el contexto activo, 
	// se obtienen dichas funciones mediante una dirección y le decimos a GLAD que cargue esas funciones en memoria desde los drivers
	// GLFW knows where the OpenGL functions are located inside the operative system(OpenGL 1.1 functions for Windows) 
	// and in the driver list of functions(above OpenGL 1.1 version). GLFW gives the address of the function list (OS + drivers)
	// to GLAD to be able to load those functions.
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::string errorMessage = "Failed to initialize GLAD";
		std::cerr << errorMessage << std::endl;
		throw std::exception(errorMessage.c_str());
	}

	centerWindow();

	disableVSync();

	onWindowResize();

	// Enable mouse movement inputs capture and the window will capture the mouse cursor
	//glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// Fuction call back for mouse inputs
	//glfwSetCursorPosCallback(m_window, mouse_callback);
	// Enable mouse button inputs
	//glfwSetMouseButtonCallback(m_window, mouse_button_callback);
}

Window::~Window() {
	close();
}

void Window::disableVSync() const {
	glfwSwapInterval(0);
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

void Window::pollEvents()
{
	glfwPollEvents();
}

GLFWwindow* Window::get()
{
	return m_window;
}

// Center the window(assuming a 1920x1080 monitor resolution)
void Window::centerWindow() {
	glfwSetWindowPos(m_window, (1920 / 2) - (m_width / 2), (1080 / 2) - (m_height / 2));
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	// Tells OpenGL the size of the render area inside the window. Indica a OpenGL el área dentro de la ventana donde va a tener que renderizar
	glViewport(0, 0, width, height);
}

void Window::onWindowResize() {
	// On window resize, call the parameter function
	glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
}



// Mouse
//double lastX = 400.0;
//double lastY = 300.0;
//float yaw = -90.0f;
//float pitch = 0.0f;
//bool firstMouse = true;
//bool zoomEnable = false;
//
//void mouse_callback(GLFWwindow* window, double xpos, double ypos)
//{
//	if (firstMouse)
//	{
//		lastX = xpos;
//		lastY = ypos;
//		firstMouse = false;
//	}
//
//	double xoffset = xpos - lastX;
//	double yoffset = lastY - ypos;
//	lastX = xpos;
//	lastY = ypos;
//
//	float sensitivity = 0.1f;
//	xoffset *= sensitivity;
//	yoffset *= sensitivity;
//
//	yaw += (float)xoffset;
//	pitch += (float)yoffset;
//
//	if (pitch > 89.0f)
//		pitch = 89.0f;
//	if (pitch < -89.0f)
//		pitch = -89.0f;
//
//	glm::vec3 direction;
//	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
//	direction.y = sin(glm::radians(pitch));
//	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
//	gameCamera.setCameraTarget(glm::normalize(direction));
//}
//
//void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
//
//	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
//		if (!zoomEnable) {
//			zoomEnable = true;
//			projectionTransform.setFOV(120.0f);
//		}
//		else {
//			zoomEnable = false;
//			projectionTransform.setFOV(90.0f);
//		}
//	}
//}