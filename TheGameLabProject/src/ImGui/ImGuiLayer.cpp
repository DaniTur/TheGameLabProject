#include "pch.h"
#include "ImGuiLayer.h"

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

// test
#include "KeyCodes.h"

ImGuiLayer::ImGuiLayer(Window& window) 
	: Layer("ImGuiLayer")
{
	m_Active = false;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGuiIO& io = ImGui::GetIO();
	io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
	io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

	ImGui_ImplGlfw_InitForOpenGL(window.get(), true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
	ImGui_ImplOpenGL3_Init("#version 330 core");
}

void ImGuiLayer::OnUpdate(double deltaTime)
{
	if (!m_Active) {
		return;
	}
	
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	//ImGuiIO& io = ImGui::GetIO();
	//io.DeltaTime = static_cast<float>(deltaTime);

	static bool show = true;
	ImGui::ShowDemoWindow();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	//LOG_TRACE("ImGuiOverlay Update");
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
	default:
		break;
	}
}

bool ImGuiLayer::IsActive() const
{
	return m_Active;
}

void ImGuiLayer::onKeyPressed(KeyPressedEvent& event)
{
	switch (event.getKeyCode())
	{
	case Key::F1:
	{
		// generate event
		ToggleLayerEvent toggleEvent("ImGuiLayer");
		m_EventCallback(toggleEvent);
		LOG_TRACE("Handled by ImGuiLayer -> KeyPressed F1");
		break;
	}
	default:
		break;
	}
	event.handled = true;
	LOG_TRACE("[ImGuiLayer] KeyPressed {}", event.getKeyCode());
}

void ImGuiLayer::SetEventCallback(const EventCallback& callback)
{
	m_EventCallback = callback;
}
