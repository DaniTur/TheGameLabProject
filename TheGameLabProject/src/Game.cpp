#include "Game.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <FPSManager.h>
#include <shader.h>
#include <Model.h>
#include <WorldTransform.h>

Game::Game() 
	: m_window(m_screenWidth, m_screenHeight), m_projectionTransform(m_screenWidth, m_screenHeight) {

	m_window.setEventCallback(std::bind(&Game::onEvent, this, std::placeholders::_1));
	
	// tell stb_image.h to flip loaded texture's on the y-axis (before loading model). Needed for importing .obj material textures
	//stbi_set_flip_vertically_on_load(true);

	glEnable(GL_DEPTH_TEST);	// Part of the Renderer
}

void Game::run() {

	m_running = true;

	// SHADERS
	// Build and Compile our shader program from files
	Shader ourShader("src/vertex_shader.vert", "src/fragment_shader.frag");

	// load models
	// -----------
	Model handGun("resources/models/handgun/1911.fbx");
	Model ourModel("resources/models/explorerBag/backpack.obj");
	Model woodenBoxes("resources/models/woodenBoxes/box.fbx");
	Model mp7("resources/models/mp7/mp7.fbx");

	WorldTransform worldTransform;
	WorldTransform handGunWorldTransform;
	WorldTransform woodenBoxesWorldTransform;
	WorldTransform mp7WorldTransform;
	
	while (m_running) {

		float fps = FPSManager::Calculate();
		//Do something with the fps
		//std::cout << "FPS: " << fps << std::endl;

		processInput(FPSManager::GetDeltaTime());

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
		glm::vec3 lightSpecular(1.0f);
		ourShader.setVec3("light.ambient", lightAmbient);
		ourShader.setVec3("light.diffuse", lightDiffuse);
		ourShader.setVec3("light.specular", lightSpecular);
		ourShader.setVec3("viewPos",m_gameCamera.getPosition());

		// Bagpack
		worldTransform.setScale(0.5f);
		glm::mat4 worldTransformMatrix = worldTransform.getMatrix();
		glm::mat4 cameraView = m_gameCamera.getView();
		glm::mat4 projectionTransformMatrix = m_projectionTransform.getMatrix();
		glm::mat4 WVPmatrix = projectionTransformMatrix * cameraView * worldTransformMatrix;
		ourShader.setMatrix4("model", worldTransformMatrix);
		ourShader.setMatrix4("view", cameraView);
		ourShader.setMatrix4("projection", projectionTransformMatrix);

		ourModel.Draw(ourShader);

		// Handgun
		handGunWorldTransform.setTranslation(glm::vec3(3.0f, 0.0f, 0.0f));
		handGunWorldTransform.setScale(0.5f);
		worldTransformMatrix = handGunWorldTransform.getMatrix();
		cameraView = m_gameCamera.getView();
		projectionTransformMatrix = m_projectionTransform.getMatrix();
		WVPmatrix = projectionTransformMatrix * cameraView * worldTransformMatrix;
		ourShader.setMatrix4("model", worldTransformMatrix);
		ourShader.setMatrix4("view", cameraView);
		ourShader.setMatrix4("projection", projectionTransformMatrix);

		handGun.Draw(ourShader);

		// Wooden Boxes
		woodenBoxesWorldTransform.setTranslation(glm::vec3(0.0f, 0.0f, -5.0f));
		worldTransformMatrix = woodenBoxesWorldTransform.getMatrix();
		cameraView = m_gameCamera.getView();
		projectionTransformMatrix = m_projectionTransform.getMatrix();
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
		cameraView = m_gameCamera.getView();
		projectionTransformMatrix = m_projectionTransform.getMatrix();
		WVPmatrix = projectionTransformMatrix * cameraView * worldTransformMatrix;
		ourShader.setMatrix4("model", worldTransformMatrix);
		ourShader.setMatrix4("view", cameraView);
		ourShader.setMatrix4("projection", projectionTransformMatrix);

		mp7.Draw(ourShader);

		// TODO: This shold be a renderer responsability?
		// Check and call IO events and swap the buffers
		m_window.swapBuffers();
		m_window.pollEvents();

	}

	// Clearing all previously allocated GLFW resources.
	glfwTerminate();
}

void Game::onEvent(Event& event)
{
	switch (event.getEventType())
	{
	case EventType::KeyPressed:
		// Obtain the GLFW keycode
		std::cout << event.toString() << std::endl;
		break;
	case EventType::KeyReleased:
		// Obtain the GLFW keycode
		std::cout << event.toString() << std::endl;
		break;
	case EventType::MouseMoved:
		// Obtain the GLFW keycode
		std::cout << event.toString() << std::endl;
		break;
	case EventType::MouseButtonPressed:
		// Obtain the GLFW keycode
		std::cout << event.toString() << std::endl;
		break;
	case EventType::MouseButtonReleased:
		// Obtain the GLFW keycode
		std::cout << event.toString() << std::endl;
		break;

	default:
		break;
	}
}

void Game::processInput(double deltaTime) {

	if (glfwGetKey(m_window.get(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(m_window.get(), true); // no se si hace falata pero lo dejo para asegurar que la ventana sabe que se tiene que cerrar
		m_running = false;
	}

	if (glfwGetKey(m_window.get(), GLFW_KEY_W) == GLFW_PRESS) {
		m_gameCamera.moveForward(static_cast<float>(deltaTime));	// TODO: Es el jugador al que hay que mover, y a la vez que se mueve el jugador, tambien la cámara
	}

	if (glfwGetKey(m_window.get(), GLFW_KEY_S) == GLFW_PRESS) {
		m_gameCamera.moveBackward(static_cast<float>(deltaTime));
	}

	if (glfwGetKey(m_window.get(), GLFW_KEY_A) == GLFW_PRESS) {
		m_gameCamera.moveLeft(static_cast<float>(deltaTime));	// TODO: Es el jugador al que hay que mover, y a la vez que se mueve el jugador, tambien la cámara
	}

	if (glfwGetKey(m_window.get(), GLFW_KEY_D) == GLFW_PRESS) {
		m_gameCamera.moveRight(static_cast<float>(deltaTime));
	}

	if (glfwGetKey(m_window.get(), GLFW_KEY_UP) == GLFW_PRESS) {
		// move up "jump"/"fly"
		m_gameCamera.moveUp(static_cast<float>(deltaTime));
	}

	if (glfwGetKey(m_window.get(), GLFW_KEY_DOWN) == GLFW_PRESS) {
		// move up "jump"/"fly"
		m_gameCamera.moveDown(static_cast<float>(deltaTime));
	}

	// Mouse movement input
	m_gameCamera.setCameraTarget(m_window.getCameraTarget());

	// Mouse buttons input
	// TODO: Hay que "consumir" el evento para que no se procese constanemente, y solo se procese una vez, sobretodo el Release
	if (m_window.getMouseButtonState(GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		std::cout << "LMB pressed, shooting\n";
	} else if (m_window.getMouseButtonState(GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
		std::cout << "LMB released\n";
		// m_window.buttonEventProcessed(GLFW_MOUSE_BUTTON_LEFT, GLFW_RELEASE);
		// 1) Iddle
		// 2) Pulsado constanemente
		// 3) Release una vez, consumimos evento y poner estado "no pulsado" al botón (-1 = no evento)?
	}
}

