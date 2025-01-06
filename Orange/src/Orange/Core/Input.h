#pragma once

#include "Orange/Core/Base.h"
#include "Orange/Core/KeyCodes.h"
#include "Orange/Core/MouseCodes.h"

namespace Orange
{
	
	class Input
	{
	public:
		static bool IsKeyPressed(KeyCode key);

		static bool IsMouseButtonPressed(MouseCode button);
		static std::pair<float, float> GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();

	};

}