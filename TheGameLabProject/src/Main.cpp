#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* window);

// Settings
const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;

// Vertex Shader source code in GLSL language
const char* vertexShaderSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";

const char* fragmentShaderOrangeSource = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\n\0";

const char* fragmentShaderYellowSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.9f, 0.0f, 1.0f);\n"
"}\n\0";

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

	// Vertex Shader
	// Create a shader object of vertex shader type
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Attach the shader source code to the shader object
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// Check if shader compilation is successful
	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Fragment Shader Orange
	unsigned int fragmentShaderOrange = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderOrange, 1, &fragmentShaderOrangeSource, NULL);
	glCompileShader(fragmentShaderOrange);
	// Check if shader compilation is successful
	glGetShaderiv(fragmentShaderOrange, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShaderOrange, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT_ORANGE::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Fragment Shader Yellow
	unsigned int fragmentShaderYellow = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderYellow, 1, &fragmentShaderYellowSource, NULL);
	glCompileShader(fragmentShaderYellow);
	// Check if shader compilation is successful
	glGetShaderiv(fragmentShaderYellow, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShaderYellow, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT_YELLOW::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Shader Program 1: with fragment shader orange
	// Final linked version of multiple shaders combined we will use to render
	unsigned int shaderProgramOrange = glCreateProgram();
	// Attach and link shaders
	glAttachShader(shaderProgramOrange, vertexShader);
	glAttachShader(shaderProgramOrange, fragmentShaderOrange);
	glLinkProgram(shaderProgramOrange);
	// Check if linking the shader program is successful
	glGetProgramiv(shaderProgramOrange, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgramOrange, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER_ORANGE::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(fragmentShaderOrange);

	// Shader Program 2: with fragment shader yellow
	unsigned int shaderProgramYellow = glCreateProgram();
	// Attach and link shaders
	glAttachShader(shaderProgramYellow, vertexShader);
	glAttachShader(shaderProgramYellow, fragmentShaderYellow);
	glLinkProgram(shaderProgramYellow);
	// Check if linking the shader program is successful
	glGetProgramiv(shaderProgramYellow, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgramYellow, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER_YELLOW::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	// If linking is successful, we can delete the shader objects
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShaderYellow);

	// Vertex Input
	// Normalized vertices input of a 2D triangle. Vertex coordinates (x,y,z) of values in [-1.0, 1.0]
	float triangleVertices1[] = {
		-1.0f, -0.5f, 0.0f, // left
		 0.0f, -0.5f, 0.0f, // right
		-0.5f,  0.5f, 0.0f  // top
	};
	// Vertices of a 2D triangle
	float triangleVertices2[] = {
		0.0f, -0.5f, 0.0f, // left
		1.0f, -0.5f, 0.0f, // right
		0.5f,  0.5f, 0.0f  // top
	};

	// Triangle 1 setup
	unsigned int VBO1;	// Vertex buffer object VBO
	unsigned int VAO1;	// Vertex Array Object VAO

	glGenVertexArrays(1, &VAO1);	// Generate the VAO
	glGenBuffers(1, &VBO1);	// Generate the VBO buffer

	glBindVertexArray(VAO1);	// Bind the VAO first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindBuffer(GL_ARRAY_BUFFER, VBO1);	// Bind the VBO

	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices1), triangleVertices1, GL_STATIC_DRAW);	// Copy vertex data to the buffer

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);	// Linking Vertex Attributes
	glEnableVertexAttribArray(0);

	// Triangle 2 setup
	unsigned int VBO2;
	unsigned int VAO2;
	
	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO2);

	glBindVertexArray(VAO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);

	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices2), triangleVertices2, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);	// Linking Vertex Attributes
	glEnableVertexAttribArray(0);

	// Render Loop
	while (!glfwWindowShouldClose(window)) {

		// Input
		process_input(window);

		// Render clear
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Rendering commands
		glUseProgram(shaderProgramOrange);

		glBindVertexArray(VAO1);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//glBindVertexArray(0);

		glUseProgram(shaderProgramYellow);
		glBindVertexArray(VAO2);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//glBindVertexArray(0);
		
		// Check and call events and swap the buffers to be rendered
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Release all resources once they've outlived their purpose
	glDeleteVertexArrays(1, &VAO1);
	glDeleteBuffers(1, &VBO1);
	glDeleteVertexArrays(1, &VAO2);
	glDeleteBuffers(1, &VBO2);
	glDeleteProgram(shaderProgramOrange);
	glDeleteProgram(shaderProgramYellow);

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