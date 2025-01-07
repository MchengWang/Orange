#pragma once

#include <glm/glm.hpp>

#include "Orange/Core/KeyCodes.h"
#include "Orange/Core/MouseCodes.h"

namespace Orange
{
	
	class Input
	{
	public:
		static bool IsKeyPressed(KeyCode key);

		static bool IsMouseButtonPressed(MouseCode button);
		static glm::vec2 GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();

	};

}