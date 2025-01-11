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
	"   gl_Position = vec4(aPos, 1.0);\n"
	"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"uniform vec4 ourColor;\n"
	"void main()\n"
	"{\n"
	"   FragColor = ourColor;\n"
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

	// Fragment Shader
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// Check if shader compilation is successful
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Shader Program
	// Final linked version of multiple shaders combined we will use to render
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	// Attach and link shaders
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// Check if linking the shader program is successful
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	// If linking is successful, we can delete the shader objects
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// Vertex Input
	// Normalized vertices input of a 2D triangle. Vertex coordinates (x,y,z) of values in [-1.0, 1.0]
	//float vertices[] = {
	//	-0.5f, -0.5f, 0.0f, // left
	//	 0.5f, -0.5f, 0.0f, // right
	//	 0.0f,  0.5f, 0.0f  // top
	//};
	// Vertices of a 2D Square
	float vertices[] = {
		 0.5f,  0.5f, 0.0f,  // top right
		 0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left 
	};

	// Select the drawing order of the vertices
	// This makes OpenGL know that it has to draw some vertices multiple times instead of adding overlapped vertices to the array
	unsigned int indices[] = {
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	// Vertex buffer object VBO
	// Is an OpenGL object used to store vertices in the GPU memory 
	unsigned int VBO;
	// Vertex Array Object VAO
	// Is an array of VBO (vertex buffer object) with all the vertex attributes info
	unsigned int VAO;
	// Elements Buffer Object
	unsigned int EBO;

	// Generate the VAO
	glGenVertexArrays(1, &VAO);
	// Generate the VBO buffer
	glGenBuffers(1, &VBO);
	// Generate the EBO
	glGenBuffers(1, &EBO);

	// Bind the VAO first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	// Bind the VBO
	// Specify the buffer type we want by binding our VBO to an OpenGL vertex buffer type
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Copy our vertex data(user defined data) into the bound buffer(GPU buffer) of type GL_ARRAY_BUFFER,
	// and the buffer usage will be GL_STATIC_DRAW(data is set only once and used many times)
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Bind the EBO
	// Specify the buffer type we want
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	// Copy our indices data(user defined data) into the bound buffer(GPU buffer)
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Linking Vertex Attributes
	// Specify what part of our input data goes to which vertex attribute in the vertex shader
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Unbind VBO, VAO and EBO
	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound VBO so afterwards we can safely unbind
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	//glBindVertexArray(0);
	// After the VAO is unbind we can unbind the EBO, because the VAO registers the EBO calls(unbind included) 
	// and if we unbind EBO first then VAO has problems unbinding an already unbinded buffer(doesn't have an EBO configured). 
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Wireframe mode
	// Configure how OpenGL draws its primitives, in that case, to draw the primitives as lines
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Render Loop
	while (!glfwWindowShouldClose(window)) {

		// Input
		process_input(window);

		// Render clear
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Rendering commands
		glUseProgram(shaderProgram);	// be sure to activate the shader

		// Set the color of the figure in our program and send it to the shader program
		// In this case the fragment shader is the one using the "ourColor" shader global variable(uniform)
		float timeValue = glfwGetTime();
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f); // sets the uniform variable value of the current used shader program

		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0); // no need to unbind it every time
		
		// Check and call IO events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Release all resources once they've outlived their purpose
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);

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