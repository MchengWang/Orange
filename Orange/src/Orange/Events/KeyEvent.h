#pragma once

#include "Orange/Events/Event.h"
#include "Orange/Core/Input.h"

namespace Orange
{
	class KeyEvent : public Event
	{
	public:
		KeyCode GetKeyCode() { return o_KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput);

	protected:
		KeyEvent(KeyCode keyCode)
			:o_KeyCode(keyCode) {}

		KeyCode o_KeyCode;
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(KeyCode keycode, int repeatCount)
		:KeyEvent(keycode), o_RepeatCount(repeatCount) {}

		int GetRepeatCount() const { return o_RepeatCount; }

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

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(KeyCode keyCode)
			:KeyEvent(keyCode) {
		}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "键盘释放事件: " << o_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};

	class KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(KeyCode keyCode)
			:KeyEvent(keyCode)
		{
		}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "按键输入事件: " << o_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped)
	};
}