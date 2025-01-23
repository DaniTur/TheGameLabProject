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

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* window);

// Settings
const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;

// World transform
WorldTransform worldTransform;

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

	// Vertex Input
	// Normalized vertices input of a 2D triangle. Vertex coordinates (x,y,z) of values in [-1.0, 1.0]
	float cubeVertices[] = {
		   // positions         // texture coords
		   -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
			0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
			0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		   -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		   -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		   -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		   -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		   -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		   -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		   -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		   -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		   -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		   -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		   -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		   -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
			0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		   -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		   -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		   -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		   -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		   -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	// Vertex buffer object VBO,  OpenGL object used to store vertices in the GPU memory 
	unsigned int VBO;
	unsigned int VAO;

	// Generate the arrays and buffers VAO, VBO, EBO
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

	// LINKING ATTRIBUTES OF THE BUFFERS
	// location, #values per vertex, type of values, normalizing values, stride between vertices attributes, offset since the buffer first position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Texture coordinates attributes
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Wireframe mode
	// Configure how OpenGL draws its primitives, in that case, to draw the primitives as lines
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// TEXTURES
	// Generate the OpenGL texture object
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// Set the texture wrapping parameters (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Load the image data to memory
	int width;
	int height;
	int numColorChannels;
	unsigned char* containerImageData = stbi_load("resources/container.jpg", &width, &height, &numColorChannels, 0);
	// Check for data loading errors
	if (containerImageData) {
		// Generate the 2D texture from the image data loaded
		//			target, mipmap level, texture format, texture width, texture height, 0, image format, image datatype, image data
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, containerImageData);
		glGenerateMipmap(GL_TEXTURE_2D);	// Generate texture copies of different resolutions
	} else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(containerImageData);	// Free the image memory

	// Render Loop
	while (!glfwWindowShouldClose(window)) {

		// Input
		process_input(window);

		// Render clear
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Rendering commands
		// First we need to pass the texture to the fragment shader via a uniform, this does it automaticaly
		//glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

		ourShader.use();
		
		// Model matrix or World Transform Matrix: model or local space to world space
		worldTransform.setRotation((float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 worldTransformMatrix = worldTransform.getMatrix();

		// View matrix or Camera Transform Matrix: world space to view space
		glm::mat4 cameraView = gameCamera.getView();
		
		// Perspective projection matrix: view space to clip space 
		glm::mat4 projectionTransformMatrix = projectionTransform.getMatrix();

		glm::mat4 WVPmatrix = projectionTransformMatrix * cameraView * worldTransformMatrix;

		ourShader.setMatrix4("WVPmatrix", WVPmatrix);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Check and call IO events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Release all resources once they've outlived their purpose
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

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

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		// move up "jump"/"fly"
	}
}
