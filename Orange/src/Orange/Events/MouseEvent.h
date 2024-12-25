#pragma once

#include "Event.h"

namespace Orange
{
	class ORANGE_API MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(float x, float y)
		:o_MouseVecX(x), o_MouseVecY(y) {}

		inline float GetVecX() const { return o_MouseVecX; }
		inline float GetVecY() const { return o_MouseVecY; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "鼠标移动事件: X = " << o_MouseVecX << ", Y = " << o_MouseVecY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

	private:
		float o_MouseVecX, o_MouseVecY;
	};

	class ORANGE_API MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(float offsetX, float offsetY)
			:o_OffsetX(offsetX), o_OffsetY(offsetY) {}

		inline float GetOffsetX() const { return o_OffsetX; }
		inline float GetOffsetY() const { return o_OffsetY; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "鼠标滚动事件: OffsetX = " << o_OffsetX << ", OffsetY = " << o_OffsetY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

	private:
		float o_OffsetX, o_OffsetY;
	};

	class ORANGE_API MouseButtonEvent : public Event
	{
	public:
		inline int GetMouseButton() const { return o_Button; }

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

	protected:
		MouseButtonEvent(int button)
			:o_Button(button) {}

		int o_Button;
	};

	class ORANGE_API MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(int button)
			:MouseButtonEvent(button) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "鼠标按下事件: 已按下" << o_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class ORANGE_API MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(int button)
			:MouseButtonEvent(button) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "鼠标释放事件: 释放" << o_Button;
			return ss.str();
		}
		
		EVENT_CLASS_TYPE(MouseButtonReleased)
	};
}