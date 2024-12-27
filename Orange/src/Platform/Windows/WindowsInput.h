#pragma once

#include "Orange/Input.h"

namespace Orange
{

	class WindowsInput : public Input
	{
	protected:
		virtual bool IsKeyPressedImpl(int keyCode) override;

		virtual bool IsMouseButtonPressedImpl(int keybutton) override;
		virtual std::pair<float, float> GetMousePositionImpl() override;
		virtual float GetMouseYImpl() override;
		virtual float GetMouseXImpl() override;
	};

}