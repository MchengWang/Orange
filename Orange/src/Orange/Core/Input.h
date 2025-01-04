#pragma once

#include "Orange/Core/Base.h"
#include "Orange/Core/KeyCodes.h"
#include "Orange/Core/MouseCodes.h"

namespace Orange
{
	
	class Input
	{
	protected:
		Input() = default;

	public:
		Input(const Input&) = delete;
		Input& operator=(const Input&) = delete;
		static bool IsKeyPressed(KeyCode keyCode) { return o_Instance->IsKeyPressedImpl(keyCode); }

		static bool IsMouseButtonPressed(MouseCode button) { return o_Instance->IsMouseButtonPressedImpl(button); }
		static std::pair<float, float> GetMousePostion() { return o_Instance->GetMousePositionImpl(); }
		static float GetMouseX() { return o_Instance->GetMouseXImpl(); }
		static float GetMouseY() { return o_Instance->GetMouseYImpl(); }

		static Scope<Input> Create();
	protected:
		virtual bool IsKeyPressedImpl(KeyCode keyCode) = 0;

		virtual bool IsMouseButtonPressedImpl(MouseCode button) = 0;
		virtual std::pair<float, float> GetMousePositionImpl() = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;
	private:
		static Scope<Input> o_Instance;
	};

}