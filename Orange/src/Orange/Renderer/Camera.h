#pragma once

#include <glm/glm.hpp>

namespace Orange
{

	class Camera
	{
	public:
		Camera(const glm::mat4& projection)
			:o_Projection(projection)
		{ }

		const glm::mat4& GetProjection() const { return o_Projection; }

	private:
		glm::mat4 o_Projection;
	};

}