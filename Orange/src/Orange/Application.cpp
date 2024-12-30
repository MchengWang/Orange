#include "ogpch.h"

#include "Application.h"

#include "Orange/Events/ApplicationEvent.h"
#include "Orange/Log.h"

#include "Orange/Renderer/Renderer.h"

#include "Input.h"

namespace Orange
{
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::o_Instance = nullptr;

	Application::Application()
		: o_Camera(-1.5f, 1.5f, -0.8f, 0.8f)
	{
		OG_CORE_ASSERT(!o_Instance, "Application 实例已经存在！")
		o_Instance = this;

		o_Window = std::unique_ptr<Window>(Window::Create());
		o_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		o_ImGuiLayer = new ImGuiLayer();
		PushOverLayer(o_ImGuiLayer);

		o_VertexArray.reset(VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};
		
		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		BufferLayout layout = {
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float4, "a_Color"}
		};
		vertexBuffer->SetLayout(layout);

		o_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		o_VertexArray->SetIndexBuffer(indexBuffer);

		o_SquareVA.reset(VertexArray::Create());

		float squareVertices[3 * 4] = {
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f, 
			-0.75f,  0.75f, 0.0f
		};

		std::shared_ptr<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ShaderDataType::Float3, "a_Position"}
			});
		o_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		o_SquareVA->SetIndexBuffer(squareIB);

		std::string vertSrc = R"(
		
			#version 430 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
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

		o_Shader.reset(new Shader(vertSrc, fragSrc));

		std::string blueShaderVertexSrc = R"(

			#version 430 core
			
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
			}

		)";

		std::string blueShaderFragmentSrc = R"(
			
			#version 430 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			void main()
			{
				color = vec4(0.2, 0.3, 0.8, 1.0);
			}

		)";

		o_BlueShader.reset(new Shader(blueShaderVertexSrc, blueShaderFragmentSrc));
	}

	Application::~Application()
	{
	}

	void Application::PushLayer(Layer* layer)
	{
		o_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverLayer(Layer* overLayer)
	{
		o_LayerStack.PushOverlay(overLayer);
		overLayer->OnAttach();
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		for (auto it = o_LayerStack.end(); it != o_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::Run()
	{
		while (o_Running)
		{
			RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
			RenderCommand::Clear();

			o_Camera.SetPosition({ 0.6f, 0.6f, 0.1f});
			o_Camera.SetRotation(30.0f);

			Renderer::BeginScene(o_Camera);

			Renderer::Submit(o_BlueShader, o_SquareVA);
			Renderer::Submit(o_Shader, o_VertexArray);

			Renderer::EndScene();

			for (Layer* layer : o_LayerStack)
				layer->OnUpdate();

			o_ImGuiLayer->Begin();
			for (Layer* layer : o_LayerStack)
				layer->OnImGuiRender();
			o_ImGuiLayer->End();

			o_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		o_Running = false;
		return true;
	}
}