#include <Orange.h>

#include "Platform/OpenGL/OpenGLShader.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public Orange::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), o_Camera(-1.5f, 1.5f, -0.8f, 0.8f), o_CameraPosition(0.0f)
	{
		o_VertexArray.reset(Orange::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		std::shared_ptr<Orange::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Orange::VertexBuffer::Create(vertices, sizeof(vertices)));
		Orange::BufferLayout layout = {
			{ Orange::ShaderDataType::Float3, "a_Position"},
			{ Orange::ShaderDataType::Float4, "a_Color"}
		};

		vertexBuffer->SetLayout(layout);
		o_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<Orange::IndexBuffer> indexBuffer;
		indexBuffer.reset(Orange::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		o_VertexArray->SetIndexBuffer(indexBuffer);

		o_SquareVA.reset(Orange::VertexArray::Create());

		float squareVertices[3 * 4] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};

		std::shared_ptr<Orange::VertexBuffer> squareVB;
		squareVB.reset(Orange::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ Orange::ShaderDataType::Float3, "a_Position"}
			});
		o_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<Orange::IndexBuffer> squareIB;
		squareIB.reset(Orange::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		o_SquareVA->SetIndexBuffer(squareIB);

		std::string vertSrc = R"(
	
			#version 430 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
			}

		)";

		std::string fragSrc = R"(
	
			#version 430 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}

		)";

		o_Shader.reset(Orange::Shader::Create(vertSrc, fragSrc));

		std::string flatColorShaderVertexSrc = R"(
	
			#version 430 core
			
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
			}

		)";

		std::string flatColorShaderFragmentSrc = R"(
	
			#version 430 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			
			uniform vec3 u_Color;

			void main()
			{
				color = vec4(u_Color, 1.0);
			}

		)";

		o_FlatColorShader.reset(Orange::Shader::Create(flatColorShaderVertexSrc, flatColorShaderFragmentSrc));
	}

	void OnUpdate(Orange::Timestep timestep) override
	{
		if (Orange::Input::IsKeyPressed(OG_KEY_LEFT))
			o_CameraPosition.x -= o_CameraMoveSpeed * timestep;
		else if (Orange::Input::IsKeyPressed(OG_KEY_RIGHT))
			o_CameraPosition.x += o_CameraMoveSpeed * timestep;

		if (Orange::Input::IsKeyPressed(OG_KEY_UP))
			o_CameraPosition.y += o_CameraMoveSpeed * timestep;
		else if (Orange::Input::IsKeyPressed(OG_KEY_DOWN))
			o_CameraPosition.y -= o_CameraMoveSpeed * timestep;

		if (Orange::Input::IsKeyPressed(OG_KEY_A))
			o_CameraRotation += o_CameraMoveSpeed * timestep;
		else if (Orange::Input::IsKeyPressed(OG_KEY_D))
			o_CameraRotation -= o_CameraMoveSpeed * timestep;

		Orange::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1});
		Orange::RenderCommand::Clear();

		o_Camera.SetPosition(o_CameraPosition);
		o_Camera.SetRotation(o_CameraRotation);

		Orange::Renderer::BeginScene(o_Camera);

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		std::dynamic_pointer_cast<Orange::OpenGLShader>(o_FlatColorShader)->Bind();
		std::dynamic_pointer_cast<Orange::OpenGLShader>(o_FlatColorShader)->UploadUniformFloat3("u_Color", o_SquareColor);

		for (int i = 0; i < 20; i++)
		{
			for (int j = 0; j < 20; j++)
			{
				glm::vec3 pos(j * 0.11f, i * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Orange::Renderer::Submit(o_FlatColorShader, o_SquareVA, transform);
			}
		}
		
		Orange::Renderer::Submit(o_Shader, o_VertexArray);

		Orange::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color:", glm::value_ptr(o_SquareColor));
		ImGui::End();
	}

	void OnEvent(Orange::Event& event) override
	{

	}

private:
	std::shared_ptr<Orange::Shader> o_Shader;
	std::shared_ptr<Orange::VertexArray> o_VertexArray;

	std::shared_ptr<Orange::Shader> o_FlatColorShader;
	std::shared_ptr<Orange::VertexArray> o_SquareVA;

	Orange::OrthographicCamera o_Camera;
	glm::vec3 o_CameraPosition;
	float o_CameraMoveSpeed = 5.0f;

	float o_CameraRotation = 0.0f;
	float o_CameraRotationSpeed = 180.0f;

	glm::vec3 o_SquareColor = { 0.2f, 0.3f, 0.8f };

};

class Sandbox : public Orange::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{

	}

};

Orange::Application* Orange::CreateApplication()
{
	return new Sandbox();
}