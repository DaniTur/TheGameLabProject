
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
	m_ActiveScene.Load();

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

	m_ActiveScene.Render(m_gameCamera);

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

	switch (event.getEventType())
	{
	using enum EventType;	// reduce verbosity
	case KeyPressed:
		onKeyPressed(static_cast<KeyPressedEvent&>(event));
		break;
	case KeyReleased:
		onKeyReleased(static_cast<KeyReleasedEvent&>(event));
		break;
	case MouseMoved:
		onMouseMoved(static_cast<MouseMovedEvent&>(event));
		break;
	case MouseButtonPressed:
		onMouseButtonPressed(static_cast<MouseButtonPressedEvent&>(event));
		break;
	case MouseButtonReleased:
		onMouseButtonReleased(static_cast<MouseButtonReleasedEvent&>(event));
		break;
	default:
		break;
	}
}

// EL input polling se llama en el Update() de Gameplay, que va antes que el Update() de ImGui,
// por tanto, se mueve la camara en la capa Gameplay aun estando con ImGui activo porque esto no son 
// eventos, sino un input polling constante. Esto se deberia parar y todos los inputs ser gestionador por ImGui?
// Entonces al activar ImGui seria imposible hacer nada en la capa de Gameplay, si queremos poder clikar los objetos 
// de las escena, le deberemos pasar la Scene a ImGui, para comprovar si el raton esta encima de un Modelo, pero ya 
// le dariamos la responsabilidad de la gestion de los clicks de la Scene a la capa de ImGui que solo debe ser responsable
// de los inputs hechos en sus ventanas, lo demas no. Digamos que es una capa que solo tiene que encargarse de lo que pasa
// con sus ventanas. Como diseno esto?
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
		// Generate event
		ToggleLayerEvent toggleEvent("ImGuiLayer");
		m_EventCallback(toggleEvent);
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
	//LOG_TRACE("[GameplayLayer] KeyPressed {}", event.getKeyCode());
}

void GameplayLayer::onKeyReleased(KeyReleasedEvent& event)
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
	case Key::DOWN:
		break;
	default:
		break;
	}
	event.handled = true;
	//LOG_TRACE("[GameplayLayer] KeyReleased {}", event.getKeyCode());
}

void GameplayLayer::onMouseMoved(MouseMovedEvent& event)
{
	float xOffset = event.getOffsetX();
	float yOffset = event.getOffsetY();

	xOffset *= m_MouseState.sensitivity;
	yOffset *= m_MouseState.sensitivity;

	// The mouse movement offset * sensitivity is equal to the angle of the camera we will move
	m_MouseState.yaw += xOffset;
	m_MouseState.pitch += yOffset;

	// Pitch limitation
	if (m_MouseState.pitch > 89.0f)
		m_MouseState.pitch = 89.0f;
	if (m_MouseState.pitch < -89.0f)
		m_MouseState.pitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(m_MouseState.yaw)) * cos(glm::radians(m_MouseState.pitch));
	direction.y = sin(glm::radians(m_MouseState.pitch));
	direction.z = sin(glm::radians(m_MouseState.yaw)) * cos(glm::radians(m_MouseState.pitch));
	m_gameCamera.setCameraTarget(glm::normalize(direction));

	event.handled = true;
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
	//LOG_TRACE("[GameplayLayer] MouseButtonPressed {}", event.getButtonCode());
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
	//LOG_TRACE("[GameplayLayer] MouseButtonReleased {}", event.getButtonCode());
}

void GameplayLayer::SetEventCallback(const EventCallback& callback)
{
	m_EventCallback = callback;
}

Scene& GameplayLayer::GetActiveScene()
{
	return m_ActiveScene;
}
