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
	o_SpriteSheet = Orange::Texture2D::Create("assets/game/textures/RPGpack_sheet_2X.png");

	o_TextureStairs = Orange::SubTexture2D::CreateFromCoords(o_SpriteSheet, { 7, 6 }, { 128, 128 });
	o_TextureBarrel = Orange::SubTexture2D::CreateFromCoords(o_SpriteSheet, { 8, 2 }, { 128, 128 });
	o_TextureTree = Orange::SubTexture2D::CreateFromCoords(o_SpriteSheet, { 2, 1 }, { 128, 128 }, { 1, 2 });

	o_Particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	o_Particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
	o_Particle.SizeBegin = 0.5f, o_Particle.SizeVariation = 0.3f, o_Particle.SizeEnd = 0.0f;
	o_Particle.LifeTime = 1.0f;
	o_Particle.Velocity = { 0.0f, 0.0f };
	o_Particle.VelocityVariation = { 3.0f, 1.0f };
	o_Particle.Position = { 0.0f, 0.0f };
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
		
#if 0
	{
		
		static float rotation = 0.0f;
		rotation += timestep * 50.0f;

		HZ_PROFILE_SCOPE("Renderer Draw");
		Orange::Renderer2D::BeginScene(o_CameraController.GetCamera());
		Orange::Renderer2D::DrawRotatedQuad({ 1.0f, 0.0f }, { 0.8f, 0.8f }, glm::radians(- 45.0f), {0.8f, 0.2f, 0.3f, 1.0f});
		Orange::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.52f, 0.21f, 0.52f, 1.0f });
		Orange::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, o_SquareColor);
		Orange::Renderer2D::DrawRotatedQuad({ 0.25f, 0.5f }, { 0.5f, 0.75f }, glm::radians(90.0f), { 0.52f, 0.21f, 0.52f, 1.0f }); // njucolor
		Orange::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 20.0f, 20.0f }, o_CheckerboardTexture, 10.0f);
		Orange::Renderer2D::DrawRotatedQuad({ -2.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, glm::radians(rotation), o_CheckerboardTexture, 20.0f);
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
#endif

	if (Orange::Input::IsMouseButtonPressed(OG_MOUSE_BUTTON_LEFT))
	{
		auto [x, y] = Orange::Input::GetMousePostion();
		auto width = Orange::Application::Get().GetWindow().GetWidth();
		auto height = Orange::Application::Get().GetWindow().GetHeight();

		auto bounds = o_CameraController.GetBounds();
		auto pos = o_CameraController.GetCamera().GetPosition();
		x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
		y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
		o_Particle.Position = { x + pos.x, y + pos.y };
		for (int i = 0; i < 5; i++)
			o_ParticleSystem.Emit(o_Particle);
	}

	o_ParticleSystem.OnUpdate(timestep);
	o_ParticleSystem.OnRender(o_CameraController.GetCamera());

	Orange::Renderer2D::BeginScene(o_CameraController.GetCamera());
	Orange::Renderer2D::DrawQuad({ 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f }, o_TextureStairs);
	Orange::Renderer2D::DrawQuad({ 1.0f, 0.0f, 1.0f }, { 1.0f, 1.0f }, o_TextureBarrel);
	Orange::Renderer2D::DrawQuad({ -1.0f, 0.0f, 1.0f }, { 1.0f, 2.0f }, o_TextureTree);
	Orange::Renderer2D::EndScene();

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