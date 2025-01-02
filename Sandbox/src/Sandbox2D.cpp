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
	o_SquareVA = Orange::VertexArray::Create();

	float squareVertices[5 * 4] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
	};

	Orange::Ref<Orange::VertexBuffer> squareVB;
	squareVB.reset(Orange::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
	squareVB->SetLayout({
		{Orange::ShaderDataType::Float3, "a_Position"}
	});
	o_SquareVA->AddVertexBuffer(squareVB);

	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
	Orange::Ref<Orange::IndexBuffer> squareIB;
	squareIB.reset(Orange::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
	o_SquareVA->SetIndexBuffer(squareIB);

	o_FlatColorShader = Orange::Shader::Create("assets/shaders/FlatColor.gsc");
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

	Orange::Renderer::BeginScene(o_CameraController.GetCamera());

	std::dynamic_pointer_cast<Orange::OpenGLShader>(o_FlatColorShader)->Bind();
	std::dynamic_pointer_cast<Orange::OpenGLShader>(o_FlatColorShader)->UploadUniformFloat4("u_Color", o_SquareColor);

	Orange::Renderer::Submit(o_FlatColorShader, o_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	Orange::Renderer::EndScene();
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