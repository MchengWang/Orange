#pragma once

#include "Orange.h"

namespace Orange
{

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnUpdate(Timestep timestep) override;
		virtual void OnImGuiRender() override;
		void OnEvent(Event& event) override;

	private:
		OrthographicCameraController o_CameraController;

		// ¡Ÿ ±
		Ref<VertexArray> o_SquareVA;
		Ref<Shader> o_FlatColorShader;
		Ref<Framebuffer> o_Framebuffer;

		Ref<Texture2D> o_CheckerboardTexture;

		glm::vec4 o_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
	};

}