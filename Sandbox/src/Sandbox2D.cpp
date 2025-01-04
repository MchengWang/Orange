#include "Sandbox2D.h"
#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

Sandbox2D::Sandbox2D()
	:Layer("Sandbox2D"), o_CameraController(1280.0f / 720.0f), o_SquareColor({ 0.2f, 0.3f, 0.8f, 1.0f })
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
	Orange::Renderer2D::ResetStats();
	{
		HZ_PROFILE_SCOPE("Renderer-Prep");
		Orange::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Orange::RenderCommand::Clear();
	}
		
	{
		
		static float rotation = 0.0f;
		rotation += timestep * 50.0f;

		HZ_PROFILE_SCOPE("Renderer Draw");
		Orange::Renderer2D::BeginScene(o_CameraController.GetCamera());
		Orange::Renderer2D::DrawRotatedQuad({ 1.0f, 0.0f }, { 0.8f, 0.8f }, -45.0f, { 0.8f, 0.2f, 0.3f, 1.0f });
		Orange::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		Orange::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, o_SquareColor);
		Orange::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.52f, 0.21f, 0.52f, 1.0f }); // njucolor
		Orange::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 20.0f, 20.0f }, o_CheckerboardTexture, 10.0f);
		Orange::Renderer2D::DrawRotatedQuad({ -2.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, rotation, o_CheckerboardTexture, 20.0f);
		Orange::Renderer2D::EndScene();

		Orange::Renderer2D::BeginScene(o_CameraController.GetCamera());
		for (float y = -5.0f; y < 5.0f; y += 0.5f)
		{
			for (float x = -5.0f; x < 5.0f; x += 0.5f)
			{
				glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.7f };
				Orange::Renderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, color);
			}
		}
		Orange::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender()
{
	HZ_PROFILE_FUNCTION();

	ImGui::Begin("Settings");
	auto stats = Orange::Renderer2D::GetStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
	ImGui::ColorEdit4("Square Color", glm::value_ptr(o_SquareColor));

	ImGui::End();
}

void Sandbox2D::OnEvent(Orange::Event& event)
{
	o_CameraController.OnEvent(event);
}