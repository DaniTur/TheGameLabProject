#define _USE_MATH_DEFINES
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "../include/shader.h"
#include "../include/stb_image.h"
#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/matrix_transform.hpp"
#include "../include/glm/gtc/type_ptr.hpp"
#include <math.h>
#include "../include/Camera.h"
#include "../include/WorldTransform.h"
#include "../include/ProjectionTransform.h"
#include "../include/Cube.h"
#include <Model.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* window);
// Mouse
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
double lastX = 400.0;
double lastY = 300.0;
float yaw = -90.0f;
float pitch = 0.0f;
bool firstMouse = true;
bool zoomEnable = false;

// Settings
const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;

// Camera
Camera gameCamera;

// Perspective projection transform
ProjectionTransform projectionTransform(SCREEN_WIDTH, SCREEN_HEIGHT);

double deltaTime = 0.0f;	// Time between current frame and last frame
double lastFrameTime = 0.0f; // Time of last frame

int main() {

	glfwInit();
	// Using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Using Core subset of features without the backwards-compatible features
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)	{

		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Center the window(assuming a 1920x1080 monitor resolution)
	glfwSetWindowPos(window, (1920/2)-(SCREEN_WIDTH/2), (1080/2)-(SCREEN_HEIGHT/2));

	glfwMakeContextCurrent(window);

	// On window resize, call the parameter function
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// GLFW tiene las funciones de OpenGL necesarias según el sistema operativo y el contexto activo, 
	// se obtienen dichas funciones mediante una dirección y le decimos a GLAD que cargue esas funciones en memoria desde los drivers
	// GLFW knows where the OpenGL functions are located inside the operative system(OpenGL 1.1 functions for Windows) 
	// and in the driver list of functions(above OpenGL 1.1 version). GLFW gives the address of the function list (OS + drivers)
	// to GLAD to be able to load those functions.
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {

		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);

	// SHADERS
	// Build and Compile our shader program from files
	Shader ourShader("src/vertex_shader.vert", "src/fragment_shader.frag");

	// load models
	// -----------
	Model ourModel("resources/models/backpack.obj");

	// Cube object
	Cube cube;
	Cube cubeStatic;

	WorldTransform worldTransform;
	WorldTransform worldTransformStatic;

	// Enable mouse movement inputs capture and the window will capture the mouse cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// Fuction call back for mouse inputs
	glfwSetCursorPosCallback(window, mouse_callback);
	// Enable mouse button inputs
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	// Render Loop
	while (!glfwWindowShouldClose(window)) {

		process_input(window);

		// Render clear
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// TODO: Change this to the correct way
		// Binding all textures to texture_unit0, not the correct way
		// and passing the sampler uniform to the shader automaticaly
		//cube.bindTexture();
		//cubeStatic.bindTexture();

		ourShader.use();
		
		// Model matrix or World Transform Matrix: model or local space to world space
		// World transform
		//WorldTransform worldTransform;
		//worldTransform.setRotation((float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));

		glm::mat4 worldTransformMatrix = worldTransform.getMatrix();
		glm::mat4 cameraView = gameCamera.getView();
		glm::mat4 projectionTransformMatrix = projectionTransform.getMatrix();
		glm::mat4 WVPmatrix = projectionTransformMatrix * cameraView * worldTransformMatrix;

		ourShader.setMatrix4("WVPmatrix", WVPmatrix);
		ourModel.Draw(ourShader);

		//cube.draw();

		//// Cube Static
		//worldTransformStatic.setTranslation(glm::vec3(3.0f, 0.0f, 0.0f));
		//worldTransformStatic.setScale(glm::vec3(0.5f, 2.0f, 0.5f));
		//worldTransformMatrix = worldTransformStatic.getMatrix();
		//cameraView = gameCamera.getView();
		//projectionTransformMatrix = projectionTransform.getMatrix();
		//WVPmatrix = projectionTransformMatrix * cameraView * worldTransformMatrix;

		//ourShader.setMatrix4("WVPmatrix", WVPmatrix);

		//cubeStatic.draw();

		// Check and call IO events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

		// Delta time calculation for Camera speed
		/*double currentFrameTime = glfwGetTime();
		deltaTime = currentFrameTime - lastFrameTime;
		lastFrameTime = currentFrameTime;
		gameCamera.setCameraSpeed((float)deltaTime);*/
	}

	// Release all resources once they've outlived their purpose
	//glDeleteVertexArrays(1, &VAO);
	//glDeleteBuffers(1, &VBO);

	// Clearing all previously allocated GLFW resources.
	glfwTerminate();
	return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	// Tells OpenGL the size of the render area inside the window. Indica a OpenGL el área dentro de la ventana donde va a tener que renderizar
	glViewport(0, 0, width, height);
}

void process_input(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		gameCamera.moveForward();
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		gameCamera.moveBackward();
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		gameCamera.moveLeft();
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		gameCamera.moveRight();
	}

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		// move up "jump"/"fly"
		gameCamera.moveUp();
	}


	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		// move up "jump"/"fly"
		gameCamera.moveDown();
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	double xoffset = xpos - lastX;
	double yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += (float)xoffset;
	pitch += (float)yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	gameCamera.setCameraTarget(glm::normalize(direction));
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {

	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
		if (!zoomEnable) {
			zoomEnable = true;
			projectionTransform.setFOV(120.0f);
		}
		else {
			zoomEnable = false;
			projectionTransform.setFOV(90.0f);
		}
	}
}

