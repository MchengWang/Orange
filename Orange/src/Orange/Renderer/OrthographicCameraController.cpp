#include "ogpch.h"
#include "OrthographicCameraController.h"

#include "Orange/Input.h"
#include "Orange/KeyCodes.h"

namespace Orange
{

	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		:o_AspectRatio(aspectRatio), o_Camera(-o_AspectRatio * o_ZoomLevel, o_AspectRatio* o_ZoomLevel, -o_ZoomLevel, o_ZoomLevel),
		o_Rotation(rotation)
	{
	}

	void OrthographicCameraController::OnUpdate(Timestep timestep)
	{
		if (Input::IsKeyPressed(OG_KEY_A))
			o_CameraPostion.x -= o_CameraTranslationSpeed * timestep;
		else if (Input::IsKeyPressed(OG_KEY_D))
			o_CameraPostion.x += o_CameraTranslationSpeed * timestep;

		if (Input::IsKeyPressed(OG_KEY_W))
			o_CameraPostion.y += o_CameraTranslationSpeed * timestep;
		else if (Input::IsKeyPressed(OG_KEY_S))
			o_CameraPostion.y -= o_CameraTranslationSpeed * timestep;

		if (o_Rotation)
		{
			if (Input::IsKeyPressed(OG_KEY_Q))
				o_CameraRotation += o_CameraRotationSpeed * timestep;
			else if (Input::IsKeyPressed(OG_KEY_E))
				o_CameraRotation -= o_CameraRotationSpeed * timestep;

			o_Camera.SetRotation(o_CameraRotation);
		}

		o_Camera.SetPosition(o_CameraPostion);

		o_CameraTranslationSpeed = o_ZoomLevel;
	}

	void OrthographicCameraController::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseScrolledEvent>(OG_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(OG_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& event)
	{ 
		o_ZoomLevel -= event.GetOffsetY() * 0.25f;
		o_ZoomLevel = std::max(o_ZoomLevel, 0.25f);
		o_Camera.SetProjection(-o_AspectRatio * o_ZoomLevel, o_AspectRatio * o_ZoomLevel, -o_ZoomLevel, o_ZoomLevel);
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& event)
	{
		o_AspectRatio = (float)event.GetWidth() / (float)event.GetHeight();
		o_Camera.SetProjection(-o_AspectRatio * o_ZoomLevel, o_AspectRatio * o_ZoomLevel, -o_ZoomLevel, o_ZoomLevel);
		return false;
	}

}