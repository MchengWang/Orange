#pragma once

#include "Event.h"

namespace Orange
{
	class ORANGE_API KeyEvent : public Event
	{
	public:
		inline int GetKeyCode() { return o_KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput);

	protected:
		KeyEvent(int KeyCode)
			:o_KeyCode(KeyCode) {}

		int o_KeyCode;
	};

	class ORANGE_API KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int keycode, int repeatCount)
		:KeyEvent(keycode), o_RepeatCount(repeatCount) {}

		inline int GetRepeatCount() const { return o_RepeatCount; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "键盘按下事件: " << o_KeyCode << " (重复: " << o_RepeatCount << " 次)";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)

	private:
		int o_RepeatCount;
	};

	class ORANGE_API KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int KeyCode)
			:KeyEvent(KeyCode) {
		}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "键盘释放事件: " << o_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};
}