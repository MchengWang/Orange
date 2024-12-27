#pragma once

#include "Orange/Core.h"

namespace Orange
{
	
	class ORANGE_API Input
	{
	public:
		inline static bool IsKeyPressed(int keyCode) { return o_Instance->IsKeyPressedImpl(keyCode); }

		inline static bool IsMouseButtonPressed(int button) { return o_Instance->IsMouseButtonPressedImpl(button); }
		inline static std::pair<float, float> GetMousePostion() { return o_Instance->GetMousePositionImpl(); }
		inline static float GetMouseX() { return o_Instance->GetMouseXImpl(); }
		inline static float GetMouseY() { return o_Instance->GetMouseYImpl(); }

	protected:
		virtual bool IsKeyPressedImpl(int keyCode) = 0;

		virtual bool IsMouseButtonPressedImpl(int button) = 0;
		virtual std::pair<float, float> GetMousePositionImpl() = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;
	private:
		static Input* o_Instance;
	};

}