#include "Sandbox2D.h"
#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

Sandbox2D::Sandbox2D()
	:Layer("Sandbox2D"), o_CameraController(1280.0f / 720.0f)
{
}

void Sandbox2D::OnAttach()
{
	HZ_PROFILE_FUNCTION();

	o_CheckerboardTexture = Orange::Texture2D::Create("assets/textures/Checkerboard.png");
}

void Sandbox2D::OnDetach()
{
	HZ_PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(Orange::Timestep timestep)
{
	HZ_PROFILE_FUNCTION();

	// ¸üÐÂ
	o_CameraController.OnUpdate(timestep);


	// äÖÈ¾
	{
		HZ_PROFILE_SCOPE("Renderer-Prep");
		Orange::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Orange::RenderCommand::Clear();
	}
		
	{
		HZ_PROFILE_SCOPE("Renderer-Draw");
		Orange::Renderer2D::BeginScene(o_CameraController.GetCamera());
		Orange::Renderer2D::DrawRotatedQuad({ -1.0f, 0.0f }, { 0.8f, -0.8f }, glm::radians(-45.0f), o_SquareColor);
		Orange::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, -0.75f }, { 0.52f, 0.21f, 0.52f, 1.0f }); // njucolor
		Orange::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, o_CheckerboardTexture, 10.284f);
		Orange::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender()
{
	HZ_PROFILE_FUNCTION();

	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(o_SquareColor));

	ImGui::End();
}

void Sandbox2D::OnEvent(Orange::Event& event)
{
	o_CameraController.OnEvent(event);
}