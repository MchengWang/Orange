#include "ogpch.h"
#include "Orange/Renderer/OrthographicCameraController.h"

#include "Orange/Core/Input.h"
#include "Orange/Core/KeyCodes.h"

namespace Orange
{

	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		:o_AspectRatio(aspectRatio), o_Camera(-o_AspectRatio * o_ZoomLevel, o_AspectRatio* o_ZoomLevel, -o_ZoomLevel, o_ZoomLevel),
		o_Rotation(rotation)
	{
	}

	void OrthographicCameraController::OnUpdate(Timestep timestep)
	{
		HZ_PROFILE_FUNCTION();

		if (Input::IsKeyPressed(OG_KEY_A))
		{
			o_CameraPostion.x -= cos(glm::radians(o_CameraRotation) * o_CameraTranslationSpeed * timestep);
			o_CameraPostion.y -= sin(glm::radians(o_CameraRotation) * o_CameraTranslationSpeed * timestep);
		}
		else if (Input::IsKeyPressed(OG_KEY_D))
		{
			o_CameraPostion.x += cos(glm::radians(o_CameraRotation) * o_CameraTranslationSpeed * timestep);
			o_CameraPostion.y += sin(glm::radians(o_CameraRotation) * o_CameraTranslationSpeed * timestep);
		}

		if (Input::IsKeyPressed(OG_KEY_W))
		{
			o_CameraPostion.x += -sin(glm::radians(o_CameraRotation) * o_CameraTranslationSpeed * timestep);
			o_CameraPostion.y += cos(glm::radians(o_CameraRotation) * o_CameraTranslationSpeed * timestep);
		}
		else if (Input::IsKeyPressed(OG_KEY_S))
		{
			o_CameraPostion.x -= -sin(glm::radians(o_CameraRotation) * o_CameraTranslationSpeed * timestep);
			o_CameraPostion.y -= cos(glm::radians(o_CameraRotation) * o_CameraTranslationSpeed * timestep);
		}

		if (o_Rotation)
		{
			if (Input::IsKeyPressed(OG_KEY_Q))
				o_CameraRotation += o_CameraRotationSpeed * timestep;
			else if (Input::IsKeyPressed(OG_KEY_E))
				o_CameraRotation -= o_CameraRotationSpeed * timestep;

			if (o_CameraRotation > 180.0f)
				o_CameraRotation -= 360.0f;
			else if (o_CameraRotation <= -180.0f)
				o_CameraRotation += 360.0f;

			o_Camera.SetRotation(o_CameraRotation);
		}

		o_Camera.SetPosition(o_CameraPostion);

		o_CameraTranslationSpeed = o_ZoomLevel;
	}

	void OrthographicCameraController::OnEvent(Event& event)
	{
		HZ_PROFILE_FUNCTION();

		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseScrolledEvent>(OG_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(OG_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& event)
	{ 
		HZ_PROFILE_FUNCTION();

		o_ZoomLevel -= event.GetOffsetY() * 0.25f;
		o_ZoomLevel = std::max(o_ZoomLevel, 0.25f);
		o_Camera.SetProjection(-o_AspectRatio * o_ZoomLevel, o_AspectRatio * o_ZoomLevel, -o_ZoomLevel, o_ZoomLevel);
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& event)
	{
		HZ_PROFILE_FUNCTION();

		o_AspectRatio = (float)event.GetWidth() / (float)event.GetHeight();
		o_Camera.SetProjection(-o_AspectRatio * o_ZoomLevel, o_AspectRatio * o_ZoomLevel, -o_ZoomLevel, o_ZoomLevel);
		return false;
	}

}