#pragma once

#include "Orange/Core/Core.h"
#include "Orange/Core/KeyCodes.h"
#include "Orange/Core/MouseCodes.h"

namespace Orange
{
	
	class Input
	{
	public:
		Input(const Input&) = delete;
		Input& operator=(const Input&) = delete;
		inline static bool IsKeyPressed(KeyCode keyCode) { return o_Instance->IsKeyPressedImpl(keyCode); }

		inline static bool IsMouseButtonPressed(MouseCode button) { return o_Instance->IsMouseButtonPressedImpl(button); }
		inline static std::pair<float, float> GetMousePostion() { return o_Instance->GetMousePositionImpl(); }
		inline static float GetMouseX() { return o_Instance->GetMouseXImpl(); }
		inline static float GetMouseY() { return o_Instance->GetMouseYImpl(); }

	protected:
		Input() = default;
		virtual bool IsKeyPressedImpl(KeyCode keyCode) = 0;

		virtual bool IsMouseButtonPressedImpl(MouseCode button) = 0;
		virtual std::pair<float, float> GetMousePositionImpl() = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;
	private:
		static Scope<Input> o_Instance;
	};

}