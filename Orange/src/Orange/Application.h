#pragma once

#include "Core.h"

#include "Window.h"

#include "Orange/LayerStack.h"
#include "Orange/Events/Event.h"
#include "Orange/Events/ApplicationEvent.h"

#include "Orange/ImGui/ImGuiLayer.h"

#include "Orange/Renderer/Shader.h"
#include "Orange/Renderer/Buffer.h"
#include "Orange/Renderer/VertexArray.h"

namespace Orange
{
	class ORANGE_API Application 
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverLayer(Layer* overLayer);

		inline Window& GetWindow() { return *o_Window; }

		inline static Application& Get() { return *o_Instance; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> o_Window;
		ImGuiLayer* o_ImGuiLayer;
		bool o_Running = true;
		LayerStack o_LayerStack;

		std::shared_ptr<Shader> o_Shader;
		std::shared_ptr<VertexArray> o_VertexArray;
		
		std::shared_ptr<Shader> o_BlueShader;
		std::shared_ptr<VertexArray> o_SquareVA;

	private:
		static Application* o_Instance;
	};

	// 在客户端定义
	Application* CreateApplication();
}