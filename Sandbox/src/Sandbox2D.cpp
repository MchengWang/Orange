#include "Sandbox2D.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

Sandbox2D::Sandbox2D()
	:Layer("Sandbox2D"), o_CameraController(1280.0f / 720.0f)
{
}

void Sandbox2D::OnAttach()
{
	
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Orange::Timestep timestep)
{
	// Update
	o_CameraController.OnUpdate(timestep);

	// Render
	Orange::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Orange::RenderCommand::Clear();
		
	Orange::Renderer2D::BeginScene(o_CameraController.GetCamera());
	Orange::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, o_SquareColor);
	Orange::Renderer2D::EndScene();

}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(o_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Orange::Event& event)
{
	o_CameraController.OnEvent(event);
}