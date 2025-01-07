#pragma once

namespace Orange
{

	typedef enum class MouseCode : uint16_t
	{
		// ю╢вт glfw3.h
		Button0 = 0,
		Button1 = 1,
		Button2 = 2,
		Button3 = 3,
		Button4 = 4,
		Button5 = 5,
		Button6 = 6,
		Button7 = 7,

		ButtonLast = Button7,
		ButtonLeft = Button0,
		ButtonRight = Button1,
		ButtonMiddle = Button2

	} Mouse;

	inline std::ostream& operator<<(std::ostream& os, MouseCode mouseCode)
	{
		os << static_cast<int32_t>(mouseCode);
		return os;
	}

}

#define OG_MOUSE_BUTTON_0      ::Orange::Mouse::Button0
#define OG_MOUSE_BUTTON_1      ::Orange::Mouse::Button1
#define OG_MOUSE_BUTTON_2      ::Orange::Mouse::Button2
#define OG_MOUSE_BUTTON_3      ::Orange::Mouse::Button3
#define OG_MOUSE_BUTTON_4      ::Orange::Mouse::Button4
#define OG_MOUSE_BUTTON_5      ::Orange::Mouse::Button5
#define OG_MOUSE_BUTTON_6      ::Orange::Mouse::Button6
#define OG_MOUSE_BUTTON_7      ::Orange::Mouse::Button7
#define OG_MOUSE_BUTTON_LAST   ::Orange::Mouse::ButtonLast
#define OG_MOUSE_BUTTON_LEFT   ::Orange::Mouse::ButtonLeft
#define OG_MOUSE_BUTTON_RIGHT  ::Orange::Mouse::ButtonRight
#define OG_MOUSE_BUTTON_MIDDLE ::Orange::Mouse::ButtonMiddle