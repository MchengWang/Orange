#pragma once

#include "Orange/Renderer/OrthographicCamera.h"
#include "Orange/Core/Timestep.h"

#include "Orange/Events/ApplicationEvent.h"
#include "Orange/Events/MouseEvent.h"

namespace Orange
{

	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);

		void OnUpdate(Timestep timestep);
		void OnEvent(Event& e);

		OrthographicCamera& GetCamera() { return o_Camera; }
		const OrthographicCamera& GetCamera() const { return o_Camera; }

		float GetZoomLevel() const { return o_ZoomLevel; }
		void SetZoomLevel(float zoomLevel) { o_ZoomLevel = zoomLevel; }

	private:
		bool OnMouseScrolled(MouseScrolledEvent& event);
		bool OnWindowResized(WindowResizeEvent& event);

	private:
		float o_AspectRatio;
		float o_ZoomLevel = 1.0f;
		OrthographicCamera o_Camera;

		bool o_Rotation;

		glm::vec3 o_CameraPostion = { 0.0f, 0.0f, 0.0f };
		float o_CameraRotation = 0.0f; // In degrees, in the anti-clockwise direction
		float o_CameraTranslationSpeed = 5.0f, o_CameraRotationSpeed = 180.0f;
	};

}