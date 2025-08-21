#include "pch.h"
#include "GameplayLayer.h"
#include "KeyCodes.h"
#include "MouseCodes.h"
#include "Events/ApplicationEvent.h"
#include "glm.hpp"
#include "Core/Input.h"
#include "Game.h"

GameplayLayer::GameplayLayer() 
	: Layer("GameplayLayer")
{
	// Shader program
	m_Shader = Shader("src/Graphics/vertex_shader.vert", "src/Graphics/fragment_shader.frag");
	// load models
	m_HandGun = Model("resources/models/handgun/1911.fbx");
	m_Bagpack = Model("resources/models/explorerBag/backpack.obj");
	m_WoodenBox = Model("resources/models/woodenBoxes/box.fbx");
	m_Mp7 = Model("resources/models/mp7/mp7.fbx");
	// TODO: Move this to Renderer
	// tell stb_image.h to flip loaded texture's on the y-axis (before loading model). Needed for importing .obj material textures
	//stbi_set_flip_vertically_on_load(true);

	glEnable(GL_DEPTH_TEST);	// Part of the Renderer, need glad to obtain the opengl functions before
	// -----
}

void GameplayLayer::OnUpdate(double deltaTime)
{
	if (!m_Active) {
		return;
	}
	// Input
	processInputPolling(deltaTime);

	// Render clear
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_Shader.use();

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

	m_Shader.setVec3("light.position", lightSourcePosition);

	// Directional light
	glm::vec3 lightDirection(-0.2f, -1.0f, -0.3f);
	m_Shader.setVec3("light.direction", lightDirection);

	// light color and intensity/strenght
	glm::vec3 lightAmbient(0.2f);
	glm::vec3 lightDiffuse(0.8f);
	glm::vec3 lightSpecular(1.0f);
	m_Shader.setVec3("light.ambient", lightAmbient);
	m_Shader.setVec3("light.diffuse", lightDiffuse);
	m_Shader.setVec3("light.specular", lightSpecular);
	m_Shader.setVec3("viewPos", m_gameCamera.getPosition());

	// Bagpack
	m_WorldTransform.setScale(0.5f);
	glm::mat4 m_WorldTransformMatrix = m_WorldTransform.getMatrix();
	glm::mat4 cameraView = m_gameCamera.getView();
	glm::mat4 projectionTransformMatrix = m_projectionTransform.getMatrix();
	glm::mat4 WVPmatrix = projectionTransformMatrix * cameraView * m_WorldTransformMatrix;
	m_Shader.setMatrix4("model", m_WorldTransformMatrix);
	m_Shader.setMatrix4("view", cameraView);
	m_Shader.setMatrix4("projection", projectionTransformMatrix);

	m_Bagpack.Draw(m_Shader);

	// Handgun
	m_handGunWorldTransform.setTranslation(glm::vec3(3.0f, 0.0f, 0.0f));
	m_handGunWorldTransform.setScale(0.5f);
	m_WorldTransformMatrix = m_handGunWorldTransform.getMatrix();
	cameraView = m_gameCamera.getView();
	projectionTransformMatrix = m_projectionTransform.getMatrix();
	WVPmatrix = projectionTransformMatrix * cameraView * m_WorldTransformMatrix;
	m_Shader.setMatrix4("model", m_WorldTransformMatrix);
	m_Shader.setMatrix4("view", cameraView);
	m_Shader.setMatrix4("projection", projectionTransformMatrix);

	m_HandGun.Draw(m_Shader);

	// Wooden Boxes
	m_woodenBoxesWorldTransform.setTranslation(glm::vec3(0.0f, 0.0f, -5.0f));
	m_WorldTransformMatrix = m_woodenBoxesWorldTransform.getMatrix();
	cameraView = m_gameCamera.getView();
	projectionTransformMatrix = m_projectionTransform.getMatrix();
	WVPmatrix = projectionTransformMatrix * cameraView * m_WorldTransformMatrix;
	m_Shader.setMatrix4("model", m_WorldTransformMatrix);
	m_Shader.setMatrix4("view", cameraView);
	m_Shader.setMatrix4("projection", projectionTransformMatrix);

	m_WoodenBox.Draw(m_Shader);

	// MP7
	m_mp7WorldTransform.setTranslation(glm::vec3(-0.5f, 0.0f, 0.0f));
	m_mp7WorldTransform.setScale(3.0f);
	m_mp7WorldTransform.setRotation(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	m_WorldTransformMatrix = m_mp7WorldTransform.getMatrix();
	cameraView = m_gameCamera.getView();
	projectionTransformMatrix = m_projectionTransform.getMatrix();
	WVPmatrix = projectionTransformMatrix * cameraView * m_WorldTransformMatrix;
	m_Shader.setMatrix4("model", m_WorldTransformMatrix);
	m_Shader.setMatrix4("view", cameraView);
	m_Shader.setMatrix4("projection", projectionTransformMatrix);

	m_Mp7.Draw(m_Shader);
}

void GameplayLayer::OnRender()
{
	if (!m_Active) {
		return;
	}
}

// GameplayLayer is always the bottom layer, so all the events that reach this layer should be always handled
void GameplayLayer::OnEvent(Event& event)
{
	if (!m_Active) {
		return;
	}

	LOG_TRACE("{}", event.toString());
	switch (event.getEventType())
	{
	using enum EventType;	// reduce verbosity
	case KeyPressed:
		onKeyPressed(static_cast<KeyPressedEvent&>(event));
		break;
	case MouseMoved:
		onMouseMoved(static_cast<MouseMovedEvent&>(event));
		break;
	default:
		break;
	}
}

bool GameplayLayer::IsActive() const
{
	return m_Active;
}

void GameplayLayer::processInputPolling(double deltaTime)
{
	if (Input::IsKeyPressed(Key::W)) {
		m_gameCamera.moveForward(static_cast<float>(deltaTime));
	}
	if (Input::IsKeyPressed(Key::A)) {
		m_gameCamera.moveLeft(static_cast<float>(deltaTime));
	}
	if (Input::IsKeyPressed(Key::S)) {
		m_gameCamera.moveBackward(static_cast<float>(deltaTime));
	}
	if (Input::IsKeyPressed(Key::D)) {
		m_gameCamera.moveRight(static_cast<float>(deltaTime));
	}
	if (Input::IsKeyPressed(Key::UP)) {
		m_gameCamera.moveUp(static_cast<float>(deltaTime));
	}
	if (Input::IsKeyPressed(Key::DOWN)) {
		m_gameCamera.moveDown(static_cast<float>(deltaTime));
	}
}

void GameplayLayer::onKeyPressed(KeyPressedEvent& event)
{
	switch (event.getKeyCode())
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
	case Key::F1:
	{
		// generate event
		ToggleLayerEvent toggleEvent("ImGuiLayer");
		m_EventCallback(toggleEvent);
		LOG_INFO("[GameplayLayer] toggle ImGuiLayer active");
		break;
	}
	case Key::ESCAPE:
	{
		// TODO: Create a new event for toggle(enable/disable) the menu. Enable/Disable layers, 
		// game should create menu if not existent, or enable/disable it if already created
		//MenuToggleEvent event;
		//m_EventCallback(event);
		WindowClosedEvent windowCloseEvent;
		m_EventCallback(windowCloseEvent);
		break;
	}
	default:
		break;
	}
	event.handled = true;
	LOG_TRACE("[GameplayLayer] KeyPressed {}", event.getKeyCode());
}

void GameplayLayer::onKeyReleased(KeyReleasedEvent& e)
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
	default:
		break;
	}
	e.handled = true;
	LOG_TRACE("KeyReleased {}", e.getKeyCode());
}

void GameplayLayer::onMouseMoved(MouseMovedEvent& event)
{
	if (m_Mouse.firstMouse)
	{
		m_Mouse.lastX = event.getX();
		m_Mouse.lastY = event.getY();
		m_Mouse.firstMouse = false;
	}

	double xoffset = event.getX() - m_Mouse.lastX;
	double yoffset = m_Mouse.lastY - event.getY();
	m_Mouse.lastX = event.getX();
	m_Mouse.lastY = event.getY();

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

	event.handled = true;
	LOG_TRACE("MouseMoved x:{} y:{}", event.getX(), event.getY());
}

void GameplayLayer::onMouseButtonPressed(MouseButtonPressedEvent& event)
{
	switch (event.getButtonCode())
	{
	case MouseBtn::ButtonLeft:
		break;
	case MouseBtn::ButtonRight:
		break;
	default:
		break;
	}
	event.handled = true;
	LOG_TRACE("ButtonPressed {}", event.getButtonCode());
}

void GameplayLayer::onMouseButtonReleased(MouseButtonReleasedEvent& event)
{
	switch (event.getButtonCode())
	{
	case MouseBtn::ButtonLeft:
		break;
	case MouseBtn::ButtonRight:
		break;
	default:
		break;
	}
	event.handled = true;
	LOG_TRACE("ButtonReleased {}", event.getButtonCode());
}

void GameplayLayer::SetEventCallback(const EventCallback& callback)
{
	m_EventCallback = callback;
}
