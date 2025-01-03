#pragma once

#include "Orange/Core/Input.h"

namespace Orange
{

	class WindowsInput : public Input
	{
	protected:
		virtual bool IsKeyPressedImpl(KeyCode keyCode) override;

		virtual bool IsMouseButtonPressedImpl(MouseCode keybutton) override;
		virtual std::pair<float, float> GetMousePositionImpl() override;
		virtual float GetMouseYImpl() override;
		virtual float GetMouseXImpl() override;
	};

}