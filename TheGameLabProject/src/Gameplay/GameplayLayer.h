#pragma once
#include "Layer.h"
#include "IEventEmitter.h"

#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
// rendering
#include "Graphics/shader.h"
#include "Graphics/Model.h"
#include "Graphics/WorldTransform.h"
#include "Graphics/ProjectionTransform.h"
#include "Graphics/Camera.h"

#include <Scene/Scene.h>

class GameplayLayer : public Layer, public IEventEmitter
{
public:
	GameplayLayer();

	void OnUpdate(double deltaTime) override;
	void OnRender() override;
	void OnEvent(Event& e) override;

	bool IsActive() const;

	void SetEventCallback(const EventCallback& callback) override;

private:
	void processInputPolling(double deltaTime);
	void onKeyPressed(KeyPressedEvent& event);
	void onKeyReleased(KeyReleasedEvent& event);
	void onMouseMoved(MouseMovedEvent& event);
	void onMouseButtonPressed(MouseButtonPressedEvent& event);
	void onMouseButtonReleased(MouseButtonReleasedEvent& event);

private:

	EventCallback m_EventCallback;

	struct Mouse {
		double lastX = 400.0;
		double lastY = 300.0;
		float yaw = -90.0f;
		float pitch = 0.0f;
		bool firstMouse = true;
		bool zoomEnable = false;
	};
	Mouse m_Mouse;

	// Rendering
	//Shader m_Shader;
	//Model m_HandGun;
	//Model m_Bagpack;
	//Model m_WoodenBox;
	//Model m_Mp7;

	//WorldTransform m_WorldTransform;
	//WorldTransform m_handGunWorldTransform;
	//WorldTransform m_woodenBoxesWorldTransform;
	//WorldTransform m_mp7WorldTransform;

	Scene m_ActiveScene;	// default Scene

	Camera m_gameCamera;
	// Perspective projection transform
	//ProjectionTransform m_projectionTransform;
};

