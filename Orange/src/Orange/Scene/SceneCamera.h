#pragma once

#include "Orange/Renderer/Camera.h"

namespace Orange
{

	class SceneCamera : public Camera
	{
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		void SetOrthographic(float size, float nearClip, float farClip);

		void SetViewportSize(uint32_t width, uint32_t height);

		float GetOrthographicSize() const { return o_OrthographicSize; }
		void SetOrthographicSize(float size) { o_OrthographicSize = size; RecalculateProjection(); }

	private:
		void RecalculateProjection();

	private:
		float o_OrthographicSize = 10.0f;
		float o_OrthographicNear = -1.0f, o_OrthographicFar = 1.0f;

		float o_AspectRatio = 0.0f;
	};

}