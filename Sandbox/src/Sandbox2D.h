#pragma once

#include "Orange.h"

class Sandbox2D : public Orange::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Orange::Timestep timestep) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Orange::Event& event) override;

private:
	Orange::OrthographicCameraController o_CameraController;

	// ¡Ÿ ±
	Orange::Ref<Orange::VertexArray> o_SquareVA;
	Orange::Ref<Orange::Shader> o_FlatColorShader;

	Orange::Ref<Orange::Texture2D> o_CheckerboardTexture;

	glm::vec4 o_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
};