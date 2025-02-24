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
#include <FPSManager.h>

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

	//glfwSwapInterval(0); // Disable vSync

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

	// tell stb_image.h to flip loaded texture's on the y-axis (before loading model). Needed for importing .obj material textures
	//stbi_set_flip_vertically_on_load(true);

	glEnable(GL_DEPTH_TEST);

	// SHADERS
	// Build and Compile our shader program from files
	Shader ourShader("src/vertex_shader.vert", "src/fragment_shader.frag");

	// load models
	// -----------
	Model handGun("resources/models/handgun/1911.fbx");
	Model ourModel("resources/models/explorerBag/backpack.obj");
	Model woodenBoxes("resources/models/woodenBoxes/box.fbx");
	//Model sr71Blackbird("resources/models/SR71/avionbien.fbx");
	//Model sr71("resources/models/Lockheed_SR-71/avionbien.fbx");
	//Model b2("resources/models/b2/B-2_v1_fbx.fbx");
	Model mp7("resources/models/mp7/mp7.fbx");
	// Cube object
	//Cube cubeStatic;

	WorldTransform worldTransform;
	WorldTransform handGunWorldTransform;
	WorldTransform woodenBoxesWorldTransform;
	//WorldTransform sr71BlackbirdWorldTransform;
	//WorldTransform sr71WorldTransform;
	//WorldTransform b2WorldTransform;
	WorldTransform mp7WorldTransform;

	// Enable mouse movement inputs capture and the window will capture the mouse cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// Fuction call back for mouse inputs
	glfwSetCursorPosCallback(window, mouse_callback);
	// Enable mouse button inputs
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	// Render Loop
	while (!glfwWindowShouldClose(window)) {

		// per-frame time logic
		float fps = FPSManager::Calculate();
		//Do something with the fps
		//std::cout << "FPS: " << fps << std::endl;

		process_input(window);

		// Render clear
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ourShader.use();
		
		// Light source
		// Point of light
		glm::vec3 lightSourcePosition(0.0f, 10.0f, 0.0f);

		glm::mat4 lightTransform(1.0f);
		glm::mat4 translate = glm::translate(lightTransform, glm::vec3(0.0f, 0.0f, 5.0f));
		glm::mat4 rotate = glm::rotate(lightTransform, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
		
		lightTransform = rotate * translate;

		glm::vec4 lightPosV4(lightSourcePosition, 1.0f);	// tranform to vec4
		lightPosV4 = lightTransform * lightPosV4; // apply transformation
		lightSourcePosition = lightPosV4;	//transform back to vec3

		ourShader.setVec3("light.position", lightSourcePosition);

		// Directional light
		glm::vec3 lightDirection(-0.2f, -1.0f, -0.3f);
		ourShader.setVec3("light.direction", lightDirection);

		// light color and intensity/strenght
		glm::vec3 lightAmbient(0.2f);
		glm::vec3 lightDiffuse(0.8f);
		glm::vec3 lightSpecular(0.1f);
		ourShader.setVec3("light.ambient", lightAmbient);
		ourShader.setVec3("light.diffuse", lightDiffuse);
		ourShader.setVec3("light.specular", lightSpecular);
		ourShader.setVec3("viewPos", gameCamera.getPosition());

		// Bagpack
		// Model matrix or World Transform Matrix: model or local space to world space
		// World transform
		//WorldTransform worldTransform;
		//worldTransform.setRotation((float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
		worldTransform.setScale(0.5f);
		glm::mat4 worldTransformMatrix = worldTransform.getMatrix();
		glm::mat4 cameraView = gameCamera.getView();
		glm::mat4 projectionTransformMatrix = projectionTransform.getMatrix();
		glm::mat4 WVPmatrix = projectionTransformMatrix * cameraView * worldTransformMatrix;
		ourShader.setMatrix4("model", worldTransformMatrix);
		ourShader.setMatrix4("view", cameraView);
		ourShader.setMatrix4("projection", projectionTransformMatrix);

		ourModel.Draw(ourShader);

		// Handgun
		handGunWorldTransform.setTranslation(glm::vec3(3.0f, 0.0f, 0.0f));
		handGunWorldTransform.setScale(0.5f);
		worldTransformMatrix = handGunWorldTransform.getMatrix();
		cameraView = gameCamera.getView();
		projectionTransformMatrix = projectionTransform.getMatrix();
		WVPmatrix = projectionTransformMatrix * cameraView * worldTransformMatrix;
		ourShader.setMatrix4("model", worldTransformMatrix);
		ourShader.setMatrix4("view", cameraView);
		ourShader.setMatrix4("projection", projectionTransformMatrix);

		handGun.Draw(ourShader);

		// Wooden Boxes
		woodenBoxesWorldTransform.setTranslation(glm::vec3(0.0f, 0.0f, -5.0f));
		worldTransformMatrix = woodenBoxesWorldTransform.getMatrix();
		cameraView = gameCamera.getView();
		projectionTransformMatrix = projectionTransform.getMatrix();
		WVPmatrix = projectionTransformMatrix * cameraView * worldTransformMatrix;
		ourShader.setMatrix4("model", worldTransformMatrix);
		ourShader.setMatrix4("view", cameraView);
		ourShader.setMatrix4("projection", projectionTransformMatrix);

		woodenBoxes.Draw(ourShader);

		// MP7
		mp7WorldTransform.setTranslation(glm::vec3(-0.5f, 0.0f, 0.0f));
		mp7WorldTransform.setScale(3.0f);
		mp7WorldTransform.setRotation(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		worldTransformMatrix = mp7WorldTransform.getMatrix();
		cameraView = gameCamera.getView();
		projectionTransformMatrix = projectionTransform.getMatrix();
		WVPmatrix = projectionTransformMatrix * cameraView * worldTransformMatrix;
		ourShader.setMatrix4("model", worldTransformMatrix);
		ourShader.setMatrix4("view", cameraView);
		ourShader.setMatrix4("projection", projectionTransformMatrix);

		mp7.Draw(ourShader);

		// SR71 Blackbird
		//sr71BlackbirdWorldTransform.setScale(0.01f);
		//sr71BlackbirdWorldTransform.setTranslation(glm::vec3(-60.0f, 0.0f, 0.0f));
		//worldTransformMatrix = sr71BlackbirdWorldTransform.getMatrix();
		//cameraView = gameCamera.getView();
		//projectionTransformMatrix = projectionTransform.getMatrix();
		//ourShader.setMatrix4("model", worldTransformMatrix);
		//ourShader.setMatrix4("view", cameraView);
		//ourShader.setMatrix4("projection", projectionTransformMatrix);

		//sr71Blackbird.Draw(ourShader);

		// SR71
		//sr71WorldTransform.setScale(0.01f);
		//sr71WorldTransform.setTranslation(glm::vec3(60.0f, 0.0f, 0.0f));
		//worldTransformMatrix = sr71WorldTransform.getMatrix();
		//cameraView = gameCamera.getView();
		//projectionTransformMatrix = projectionTransform.getMatrix();
		//ourShader.setMatrix4("model", worldTransformMatrix);
		//ourShader.setMatrix4("view", cameraView);
		//ourShader.setMatrix4("projection", projectionTransformMatrix);

		//sr71.Draw(ourShader);

		//b2WorldTransform.setScale(0.1f);
		//b2WorldTransform.setTranslation(glm::vec3(20.0f, 0.0f, 0.0f));
		//worldTransformMatrix = b2WorldTransform.getMatrix();
		//cameraView = gameCamera.getView();
		//projectionTransformMatrix = projectionTransform.getMatrix();
		//ourShader.setMatrix4("model", worldTransformMatrix);
		//ourShader.setMatrix4("view", cameraView);
		//ourShader.setMatrix4("projection", projectionTransformMatrix);

		//b2.Draw(ourShader);

		// Light cube
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

