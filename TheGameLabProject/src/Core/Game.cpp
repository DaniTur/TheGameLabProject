#include "pch.h"

#include "Game.h"
#include "FPSManager.h"
#include "Graphics/shader.h"
#include "Graphics/Model.h"
#include "Graphics/WorldTransform.h"
#include "Events/MouseEvent.h"
#include "Input.h"
#include "ImGui/ImGuiLayer.h"
#include "Gameplay/GameplayLayer.h"

Game::Game() 
	: m_window(m_screenWidth, m_screenHeight), m_projectionTransform(m_screenWidth, m_screenHeight) {

	m_window.setEventCallback(std::bind_front(&Game::onEvent, this));

	Input::Init(m_window.get());

	m_LayerStack.PushLayer(new GameplayLayer());
	m_LayerStack.PushOverlay(new ImGuiLayer());
	

	// TODO: Move this to Renderer
	// tell stb_image.h to flip loaded texture's on the y-axis (before loading model). Needed for importing .obj material textures
	//stbi_set_flip_vertically_on_load(true);

	glEnable(GL_DEPTH_TEST);	// Part of the Renderer, need glad to obtain the opengl functions before
	// -----
}

void Game::run() {

	m_running = true;
	LOG_INFO("Game running...");

	// SHADERS
	// Build and Compile our shader program from files
	Shader ourShader("src/Graphics/vertex_shader.vert", "src/Graphics/fragment_shader.frag");

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

		// Timing
		FPSManager::Calculate();
		m_DeltaTime = FPSManager::GetDeltaTime();

		processInputPolling();

		// Update
		//for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); it--) {
		//	(*it)->OnUpdate(m_DeltaTime);
		//}

		//// Render
		//for (auto& layer : m_LayerStack) {
		//	layer->OnRender();
		//}


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

		onUpdate();

		m_window.onUpdate();
	}

	// Clearing all previously allocated GLFW resources.
	glfwTerminate();
}

void Game::onEvent(Event& event)
{
	if (event.getEventType() == EventType::WindowClosed)
		onWindowClosed(static_cast<WindowClosedEvent&>(event));

	for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it) {
		if (event.handled)
			break;
	
		(*it)->OnEvent(event);
	}
}

void Game::processInputPolling()
{
	if (Input::IsKeyPressed(Key::W)) {	
		m_gameCamera.moveForward(static_cast<float>(m_DeltaTime));
	}
	if (Input::IsKeyPressed(Key::A)) {
		m_gameCamera.moveLeft(static_cast<float>(m_DeltaTime));
	}
	if (Input::IsKeyPressed(Key::S)) {
		m_gameCamera.moveBackward(static_cast<float>(m_DeltaTime));
	}
	if (Input::IsKeyPressed(Key::D)) {
		m_gameCamera.moveRight(static_cast<float>(m_DeltaTime));
	}
	if (Input::IsKeyPressed(Key::UP)) {
		m_gameCamera.moveUp(static_cast<float>(m_DeltaTime));
	}
	if (Input::IsKeyPressed(Key::DOWN)) {
		m_gameCamera.moveDown(static_cast<float>(m_DeltaTime));
	}
}

void Game::onUpdate()
{
	// check for input key presses

}

void Game::onKeyPressed(KeyPressedEvent& e)
{
	switch (e.getKeyCode())
	{
	case Key::W:
		break;
	case Key::A:
		break;
	case Key::S:
		break;
	case Key::D:
		break;
	case Key::UP:
		break;
	case Key::DOWN:
		break;
	case Key::ESCAPE:
	{
		WindowClosedEvent event;
		onWindowClosed(event);
		break;
	}
	default:
		break;
	}
	e.handled = true;
	LOG_TRACE("KeyPressed {}", e.getKeyCode());
}



void Game::onMouseMoved(MouseMovedEvent &e) {
	// TODO: Mouse input pulling, or mouse event for movement? 
	if (m_Mouse.firstMouse)
	{
		m_Mouse.lastX = e.getX();
		m_Mouse.lastY = e.getY();
		m_Mouse.firstMouse = false;
	}

	double xoffset = e.getX() - m_Mouse.lastX;
	double yoffset = m_Mouse.lastY - e.getY();
	m_Mouse.lastX = e.getX();
	m_Mouse.lastY = e.getY();

	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	// The mouse movement offset is equal to the angle of the camera we will move
	m_Mouse.yaw += (float)xoffset;
	m_Mouse.pitch += (float)yoffset;

	// Pitch limitation
	if (m_Mouse.pitch > 89.0f)
		m_Mouse.pitch = 89.0f;
	if (m_Mouse.pitch < -89.0f)
		m_Mouse.pitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(m_Mouse.yaw)) * cos(glm::radians(m_Mouse.pitch));
	direction.y = sin(glm::radians(m_Mouse.pitch));
	direction.z = sin(glm::radians(m_Mouse.yaw)) * cos(glm::radians(m_Mouse.pitch));
	m_gameCamera.setCameraTarget(glm::normalize(direction));
	
	e.handled = true;
	//LOG_TRACE("MouseMoved x:{} y:{}", e.getX(), e.getY());
}



void Game::onWindowClosed(WindowClosedEvent& e)
{
	m_running = false;
	e.handled = true;
}