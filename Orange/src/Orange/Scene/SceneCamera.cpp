#include "ogpch.h"
#include "SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Orange
{

	SceneCamera::SceneCamera()
	{
		RecalculateProjection();
	}

	void SceneCamera::SetOrthographic(float size, float nearClip, float farClip)
	{
		o_OrthographicSize = size;
		o_OrthographicNear = nearClip;
		o_OrthographicFar = farClip;

		RecalculateProjection();
	}

	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		o_AspectRatio = (float)width / (float)height;
		RecalculateProjection();
	}

	void SceneCamera::RecalculateProjection()
	{
		float orthoLeft = -o_OrthographicSize * o_AspectRatio * 0.5f;
		float orthoRight =  o_OrthographicSize * o_AspectRatio * 0.5f;
		float orthoBottom = -o_OrthographicSize * 0.5f;
		float orthoTop =  o_OrthographicSize * 0.5f;

		o_Projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop,
			o_OrthographicNear, o_OrthographicFar);
	}

}