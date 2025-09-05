#include "pch.h"
#include "ImGuiLayer.h"

#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

// test
#include "KeyCodes.h"

ImGuiLayer::ImGuiLayer(Window& window, Scene& scene)
	: Layer("ImGuiLayer"), m_Window(window), m_ActiveScene(scene)
{
	m_Active = false;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGuiIO& io = ImGui::GetIO();
	io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors; // Automatically change cursors(arrow, hand, resize arrows, etc)
	io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;  // Permits ImGui to move the mouse when the library needs

	//ImGui_ImplGlfw_InitForOpenGL(m_Window.get(), true);
	ImGui_ImplOpenGL3_Init("#version 330 core");
}

ImGuiLayer::~ImGuiLayer()
{
	//ImGui_ImplGlfw_Shutdown();
	ImGui_ImplOpenGL3_Shutdown();	
	ImGui::DestroyContext();
}

void ImGuiLayer::OnUpdate(double deltaTime)
{
	if (!m_Active) {
		return;
	}
	
	ImGuiIO& io = ImGui::GetIO();
	io.DeltaTime = static_cast<float>(deltaTime);
	io.DisplaySize = ImVec2((float)m_Window.GetWidth(), (float)m_Window.GetHeight());

	//ImGui_ImplGlfw_NewFrame();
	ImGui_ImplOpenGL3_NewFrame();
	ImGui::NewFrame();

	glm::vec3 position(0.0f);
	glm::vec3 rotation(0.0f);
	glm::vec3 scale(0.0f);
	// Improve Readability
	const int X = 0;	
	const int Y = 1;
	const int Z = 2;

	ImGui::ShowDemoWindow();

	ImGui::Begin("Inspector");
	ImGui::SeparatorText("Transform");
	for (auto gameObject : m_ActiveScene) {
		GameObjectData& gameObjectData = gameObject->GetData();
		std::string nameText = gameObjectData.name;

		if (ImGui::CollapsingHeader(nameText.c_str())) {
			if (ImGui::BeginTable(nameText.c_str(), 4, ImGuiTableFlags_SizingStretchProp)) {
				// Position row
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("Position");

				ImGui::TableSetColumnIndex(1);
				ImGui::Text("X");
				ImGui::SameLine();
				ImGui::InputFloat(("##posX" + nameText).c_str(), &gameObjectData.position[X]);

				ImGui::TableSetColumnIndex(2);
				ImGui::Text("Y");
				ImGui::SameLine();
				ImGui::InputFloat(("##posY" + nameText).c_str(), &gameObjectData.position[Y]);

				ImGui::TableSetColumnIndex(3);
				ImGui::Text("Z");
				ImGui::SameLine();
				ImGui::InputFloat(("##posZ" + nameText).c_str(), &gameObjectData.position[Z]);

				// Rotation row
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("Rotation");

				ImGui::TableSetColumnIndex(1);
				ImGui::Text("X");
				ImGui::SameLine();
				ImGui::InputFloat(("##rotX" + nameText).c_str(), &gameObjectData.rotation[X]);

				ImGui::TableSetColumnIndex(2);
				ImGui::Text("Y");
				ImGui::SameLine();
				ImGui::InputFloat(("##rotY" + nameText).c_str(), &gameObjectData.rotation[Y]);

				ImGui::TableSetColumnIndex(3);
				ImGui::Text("Z");
				ImGui::SameLine();
				ImGui::InputFloat(("##rotZ" + nameText).c_str(), &gameObjectData.rotation[Z]);

				// Scale row
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("Scale");

				ImGui::TableSetColumnIndex(1);
				ImGui::Text("X");
				ImGui::SameLine();
				ImGui::InputFloat(("##scaX" + nameText).c_str(), &gameObjectData.scale[X]);

				ImGui::TableSetColumnIndex(2);
				ImGui::Text("Y");
				ImGui::SameLine();
				ImGui::InputFloat(("##scaY" + nameText).c_str(), &gameObjectData.scale[Y]);

				ImGui::TableSetColumnIndex(3);
				ImGui::Text("Z");
				ImGui::SameLine();
				ImGui::InputFloat(("##scaZ" + nameText).c_str(), &gameObjectData.scale[Z]);

				ImGui::EndTable();
			}
		}
	}
	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiLayer::OnRender()
{
	if (!m_Active) {
		return;
	}
}

void ImGuiLayer::OnEvent(Event& event)
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
	case MouseScrolled:
		onMouseScrolled(static_cast<MouseScrolledEvent&>(event));
		break;
	default:
		break;
	}
}

void ImGuiLayer::onKeyPressed(KeyPressedEvent& event)
{
	if (event.getKeyCode() == Key::F1) {
		ToggleLayerEvent toggleEvent("ImGuiLayer");
		m_EventCallback(toggleEvent);
	}

	ImGuiIO& io = ImGui::GetIO();
	ImGuiKey key = KeyMapToImGuiKey(event.getKeyCode());
	io.AddKeyEvent(key, true);
	event.handled = true;
	LOG_TRACE("[ImGuiLayer] KeyPressed {}", event.getKeyCode());
}

void ImGuiLayer::onKeyReleased(KeyReleasedEvent& event) {
	ImGuiIO& io = ImGui::GetIO();
	ImGuiKey key = KeyMapToImGuiKey(event.getKeyCode());
	io.AddKeyEvent(key, false);
	event.handled = true;
	LOG_TRACE("[ImGuiLayer] KeyReleased {}", event.getKeyCode());
}

void ImGuiLayer::onMouseMoved(MouseMovedEvent& event) {
	ImGuiIO& io = ImGui::GetIO();
	io.AddMousePosEvent(event.getX(), event.getY());
	event.handled = true;
}

void ImGuiLayer::onMouseButtonPressed(MouseButtonPressedEvent& event) {
	ImGuiIO& io = ImGui::GetIO();
	io.AddMouseButtonEvent(event.getButtonCode(), true);
	event.handled = true;
	LOG_TRACE("[ImGuiLayer] MouseButtonPressed {}", event.getButtonCode());
}

void ImGuiLayer::onMouseButtonReleased(MouseButtonReleasedEvent& event) {
	ImGuiIO& io = ImGui::GetIO();
	io.AddMouseButtonEvent(event.getButtonCode(), false);
	event.handled = true;
	LOG_TRACE("[ImGuiLayer] MouseButtonReleased {}", event.getButtonCode());
}

void ImGuiLayer::onMouseScrolled(MouseScrolledEvent& event)
{
	ImGuiIO& io = ImGui::GetIO();
	io.AddMouseWheelEvent(event.GetXOffset(), event.GetYOffset());
	event.handled = true;
}

ImGuiKey ImGuiLayer::KeyMapToImGuiKey(int nativeKeyCode)
{
	switch (nativeKeyCode)
	{
	case Key::W: return ImGuiKey_W;
	case Key::A: return ImGuiKey_A;
	case Key::S: return ImGuiKey_S;
	case Key::D: return ImGuiKey_D;
	default: return ImGuiKey_None;
	}
}

void ImGuiLayer::SetEventCallback(const EventCallback& callback)
{
	m_EventCallback = callback;
}

void ImGuiLayer::SetScene(Scene& scene)
{
	m_ActiveScene = scene;
}
