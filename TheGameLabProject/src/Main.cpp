#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "../include/shader.h"
#include "../include/stb_image.h"
#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/matrix_transform.hpp"
#include "../include/glm/gtc/type_ptr.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* window);

// Settings
const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;

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

	// SHADERS
	// Build and Compile our shader program from files
	Shader ourShader("src/vertex_shader.vert", "src/fragment_shader.frag");

	// Vertex Input
	// Normalized vertices input of a 2D triangle. Vertex coordinates (x,y,z) of values in [-1.0, 1.0]
	float vertices[] = {
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
	};
	int indices[] = {
		2, 3, 0,
		2, 1, 0
	};

	// Vertex buffer object VBO
	// Is an OpenGL object used to store vertices in the GPU memory 
	unsigned int VBO;
	// Vertex Array Object VAO
	// Is an array of VBO (vertex buffer object) with all the vertex attributes info
	unsigned int VAO;
	unsigned int EBO;

	// Generate the VAO
	glGenVertexArrays(1, &VAO);
	// Generate the VBO buffer
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Bind the VAO first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	// Bind the VBO
	// Specify the buffer type we want by binding our VBO to an OpenGL vertex buffer type
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Copy our vertex data(user defined data) into the bound buffer(GPU buffer) of type GL_ARRAY_BUFFER,
	// and the buffer usage will be GL_STATIC_DRAW(data is set only once and used many times)
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// LINKING ATTRIBUTES OF THE BUFFER for shaders to use
	// Specify what part of our input data goes to which vertex attribute in the vertex shader
	// Vertex position attributes
	// location, #values per vertex, type of values, normalizing values, stride between vertices attributes, offset since the buffer first position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// Vertices Color attributes
	// location, #values per color, type of values, normalizing values, stride between color attributes, offset since the buffer first position
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// Texture coordinates attributes
	// location, #values per texture coordinate, type of values, normalizing values, stride between texture coordinate attributes, offset since the buffer first position
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// Wireframe mode
	// Configure how OpenGL draws its primitives, in that case, to draw the primitives as lines
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// TEXTURES
	// Generate the OpenGL texture object
	unsigned int texture;
	glGenTextures(1, &texture);
	// Bind the texture object so the target of GL_TEXTURE_2D is our texture object
	glBindTexture(GL_TEXTURE_2D, texture);
	// Set the texture wrapping parameters (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
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

	// TRANSFORMATION
	// Translation
	//glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);								// The vector(vertex) we want to translate
	glm::mat4 translation(1.0f);							// Create translation matrix (by default an identity matrix)
	translation = glm::translate(translation, glm::vec3(0.25f, 0.25f, 0.0f));	// Set translation values for x,y,z into the translation matrix(verticaly)
	//vec = translation * vec;											// Left dot product translation matrix with the vector we want to translate

	// Scaling
	//glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);						// The vector(vertex) we want to scale
	glm::mat4 scaling(1.0f);						// Create the default identity matrix we will use to scale
	scaling = glm::scale(scaling, glm::vec3(0.5, 0.5, 0.5));	// Set the scaling values for x,y,z into the matrix(diagonaly) 
	//vec = scaling * vec;										// Left dot product scaling matrix with the vector we want to scale

	// Rotation
	//glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);												// The vector(vertex) we want to rotate
	glm::mat4 rotation(1.0f);												// Create the default identity matrix we will use to rotate
	rotation = glm::rotate(rotation, glm::radians(45.0f), glm::vec3(0.0, 0.0, 1.0));	// Set rotation values 90º(positive: to the left/counter clockwise) in Z axis(0,0,1)
	//vec = rotation * vec;																// Left dot product rotation matrix with the vector we want to rotate

	/* We can pass the transformation matrix to the vertex shader to let the GPU make the operation faster	*/
	// unsigned int transformMatrixLocation = glGetUniformLocation(ourShader.ID, "transformMatrix");
	// glUniformMatrix4fv(transformMatrixLocation, 1, GL_FALSE, glm::value_ptr(trans));

	// Translation, Scaling and Rotation
	glm::mat4 transformationMatrix = rotation * translation;
	

	// Render Loop
	while (!glfwWindowShouldClose(window)) {

		// Input
		process_input(window);

		// Render clear
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Rendering commands
		// First we need to pass the texture to the fragment shader via a uniform, this does it automaticaly
		glBindTexture(GL_TEXTURE_2D, texture);

		ourShader.use();
		glUniformMatrix4fv(glGetUniformLocation(ourShader.ID, "transformMatrix"), 1, GL_FALSE, glm::value_ptr(transformationMatrix));

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
		// Check and call IO events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Release all resources once they've outlived their purpose
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

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
}