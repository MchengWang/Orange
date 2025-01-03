#pragma once 

namespace Orange
{

	typedef enum class KeyCode : uint16_t
	{
		// From glfw3.h
		Space = 32,
		Apostrophe = 39, /* ' */
		Comma = 44, /* , */
		Minus = 45, /* - */
		Dot = 46, /* . */
		Slash = 47, /* / */

		// 数字
		N0 = 48, /* 0 */
		N1 = 49, /* 1 */
		N2 = 50, /* 2 */
		N3 = 51, /* 3 */
		N4 = 52, /* 4 */
		N5 = 53, /* 5 */
		N6 = 54, /* 6 */
		N7 = 55, /* 7 */
		N8 = 56, /* 8 */
		N9 = 57, /* 9 */
		Semicolon = 59, /* ; */
		Equal = 61, /* = */

		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72,
		I = 73,
		J = 74,
		K = 75,
		L = 76,
		M = 77,
		N = 78,
		O = 79,
		P = 80,
		Q = 81,
		R = 82,
		S = 83,
		T = 84,
		U = 85,
		V = 86,
		W = 87,
		X = 88,
		Y = 89,
		Z = 90,

		LeftBracket = 91,  /* [ */
		Backslash = 92,  /* \ */
		RightBracket = 93,  /* ] */
		GraveAccent = 96,  /* ` */
		World1 = 161, /* non-US #1 */
		World2 = 162, /* non-US #2 */

		/* 功能键 */
		Esc = 256, // 返回
		Enter = 257, // 确认
		Tab = 258,
		Backspace = 259, // 回退
		Insert = 260, // 插入
		Delete = 261, // 删除
		Right = 262,
		Left = 263,
		Down = 264,
		Up = 265,
		PageUp = 266,
		PageDown = 267,
		Home = 268,
		End = 269,
		CapsLock = 280,
		ScrollLock = 281,
		NumLock = 282,
		PrintScreen = 283,
		Pause = 284,

		F1 = 290,
		F2 = 291,
		F3 = 292,
		F4 = 293,
		F5 = 294,
		F6 = 295,
		F7 = 296,
		F8 = 297,
		F9 = 298,
		F10 = 299,
		F11 = 300,
		F12 = 301,
		F13 = 302,
		F14 = 303,
		F15 = 304,
		F16 = 305,
		F17 = 306,
		F18 = 307,
		F19 = 308,
		F20 = 309,
		F21 = 310,
		F22 = 311,
		F23 = 312,
		F24 = 313,
		F25 = 314,

		/* Keypad */
		KP0 = 320,
		KP1 = 321,
		KP2 = 322,
		KP3 = 323,
		KP4 = 324,
		KP5 = 325,
		KP6 = 326,
		KP7 = 327,
		KP8 = 328,
		KP9 = 329,
		KPDecimal = 330,
		KPDivide = 331,
		KPMultiply = 332,
		KPSubtract = 333,
		KPAdd = 334,
		KPEnter = 335,
		KPEqual = 336,
		LeftShift = 340,
		LeftControl = 341,
		LeftAlt = 342,
		LeftSuper = 343,
		RightShift = 344,
		RightControl = 345,
		RightAlt = 346,
		RightSuper = 347,
		Menu = 348

	} Key;

	inline std::ostream& operator<<(std::ostream& os, KeyCode keyCode)
	{
		os << static_cast<int32_t>(keyCode);
		return os;
	}

}

#define OG_KEY_SPACE           ::Orange::Key::Space
#define OG_KEY_APOSTROPHE      ::Orange::Key::Apostrophe    /* ' */
#define OG_KEY_COMMA           ::Orange::Key::Comma         /* , */
#define OG_KEY_MINUS           ::Orange::Key::Minus         /* - */
#define OG_KEY_DOT             ::Orange::Key::Dot        /* . */
#define OG_KEY_SLASH           ::Orange::Key::Slash         /* / */

#define OG_KEY_0               ::Orange::Key::N0
#define OG_KEY_1               ::Orange::Key::N1
#define OG_KEY_2               ::Orange::Key::N2
#define OG_KEY_3               ::Orange::Key::N3
#define OG_KEY_4               ::Orange::Key::N4
#define OG_KEY_5               ::Orange::Key::N5
#define OG_KEY_6               ::Orange::Key::N6
#define OG_KEY_7               ::Orange::Key::N7
#define OG_KEY_8               ::Orange::Key::N8
#define OG_KEY_9               ::Orange::Key::N9

#define OG_KEY_SEMICOLON       ::Orange::Key::Semicolon     /* ; */
#define OG_KEY_EQUAL           ::Orange::Key::Equal         /* = */

#define OG_KEY_A               ::Orange::Key::A
#define OG_KEY_B               ::Orange::Key::B
#define OG_KEY_C               ::Orange::Key::C
#define OG_KEY_D               ::Orange::Key::D
#define OG_KEY_E               ::Orange::Key::E
#define OG_KEY_F               ::Orange::Key::F
#define OG_KEY_G               ::Orange::Key::G
#define OG_KEY_H               ::Orange::Key::H
#define OG_KEY_I               ::Orange::Key::I
#define OG_KEY_J               ::Orange::Key::J
#define OG_KEY_K               ::Orange::Key::K
#define OG_KEY_L               ::Orange::Key::L
#define OG_KEY_M               ::Orange::Key::M
#define OG_KEY_N               ::Orange::Key::N
#define OG_KEY_O               ::Orange::Key::O
#define OG_KEY_P               ::Orange::Key::P
#define OG_KEY_Q               ::Orange::Key::Q
#define OG_KEY_R               ::Orange::Key::R
#define OG_KEY_S               ::Orange::Key::S
#define OG_KEY_T               ::Orange::Key::T
#define OG_KEY_U               ::Orange::Key::U
#define OG_KEY_V               ::Orange::Key::V
#define OG_KEY_W               ::Orange::Key::W
#define OG_KEY_X               ::Orange::Key::X
#define OG_KEY_Y               ::Orange::Key::Y
#define OG_KEY_Z               ::Orange::Key::Z

#define OG_KEY_LEFT_BRACKET    ::Orange::Key::LeftBracket   /* [ */
#define OG_KEY_BACKSLASH       ::Orange::Key::Backslash     /* \ */
#define OG_KEY_RIGHT_BRACKET   ::Orange::Key::RightBracket  /* ] */
#define OG_KEY_GRAVE_ACCENT    ::Orange::Key::GraveAccent   /* ` */
#define OG_KEY_WORLD_1         ::Orange::Key::World1        /* non-US #1 */
#define OG_KEY_WORLD_2         ::Orange::Key::World2        /* non-US #2 */

#define OG_KEY_ESC             ::Orange::Key::Esc
#define OG_KEY_ENTER           ::Orange::Key::Enter
#define OG_KEY_TAB             ::Orange::Key::Tab
#define OG_KEY_BACKSPACE       ::Orange::Key::Backspace
#define OG_KEY_INSERT          ::Orange::Key::Insert
#define OG_KEY_DELETE          ::Orange::Key::Delete
#define OG_KEY_RIGHT           ::Orange::Key::Right
#define OG_KEY_LEFT            ::Orange::Key::Left
#define OG_KEY_DOWN            ::Orange::Key::Down
#define OG_KEY_UP              ::Orange::Key::Up
#define OG_KEY_PAGE_UP         ::Orange::Key::PageUp
#define OG_KEY_PAGE_DOWN       ::Orange::Key::PageDown
#define OG_KEY_HOME            ::Orange::Key::Home
#define OG_KEY_END             ::Orange::Key::End
#define OG_KEY_CAPS_LOCK       ::Orange::Key::CapsLock
#define OG_KEY_SCROLL_LOCK     ::Orange::Key::ScrollLock
#define OG_KEY_NUM_LOCK        ::Orange::Key::NumLock
#define OG_KEY_PRINT_SCREEN    ::Orange::Key::PrintScreen
#define OG_KEY_PAUSE           ::Orange::Key::Pause

// 功能键
#define OG_KEY_F1              ::Orange::Key::F1
#define OG_KEY_F2              ::Orange::Key::F2
#define OG_KEY_F3              ::Orange::Key::F3
#define OG_KEY_F4              ::Orange::Key::F4
#define OG_KEY_F5              ::Orange::Key::F5
#define OG_KEY_F6              ::Orange::Key::F6
#define OG_KEY_F7              ::Orange::Key::F7
#define OG_KEY_F8              ::Orange::Key::F8
#define OG_KEY_F9              ::Orange::Key::F9
#define OG_KEY_F10             ::Orange::Key::F10
#define OG_KEY_F11             ::Orange::Key::F11
#define OG_KEY_F12             ::Orange::Key::F12
#define OG_KEY_F13             ::Orange::Key::F13
#define OG_KEY_F14             ::Orange::Key::F14
#define OG_KEY_F15             ::Orange::Key::F15
#define OG_KEY_F16             ::Orange::Key::F16
#define OG_KEY_F17             ::Orange::Key::F17
#define OG_KEY_F18             ::Orange::Key::F18
#define OG_KEY_F19             ::Orange::Key::F19
#define OG_KEY_F20             ::Orange::Key::F20
#define OG_KEY_F21             ::Orange::Key::F21
#define OG_KEY_F22             ::Orange::Key::F22
#define OG_KEY_F23             ::Orange::Key::F23
#define OG_KEY_F24             ::Orange::Key::F24
#define OG_KEY_F25             ::Orange::Key::F25

/* Keypad */
#define OG_KEY_KP_0            ::Orange::Key::KP0
#define OG_KEY_KP_1            ::Orange::Key::KP1
#define OG_KEY_KP_2            ::Orange::Key::KP2
#define OG_KEY_KP_3            ::Orange::Key::KP3
#define OG_KEY_KP_4            ::Orange::Key::KP4
#define OG_KEY_KP_5            ::Orange::Key::KP5
#define OG_KEY_KP_6            ::Orange::Key::KP6
#define OG_KEY_KP_7            ::Orange::Key::KP7
#define OG_KEY_KP_8            ::Orange::Key::KP8
#define OG_KEY_KP_9            ::Orange::Key::KP9

#define OG_KEY_KP_DECIMAL      ::Orange::Key::KPDecimal
#define OG_KEY_KP_DIVIDE       ::Orange::Key::KPDivide
#define OG_KEY_KP_MULTIPLY     ::Orange::Key::KPMultiply
#define OG_KEY_KP_SUBTRACT     ::Orange::Key::KPSubtract
#define OG_KEY_KP_ADD          ::Orange::Key::KPAdd
#define OG_KEY_KP_ENTER        ::Orange::Key::KPEnter
#define OG_KEY_KP_EQUAL        ::Orange::Key::KPEqual
#define OG_KEY_LEFT_SHIFT      ::Orange::Key::LeftShift
#define OG_KEY_LEFT_CONTROL    ::Orange::Key::LeftControl
#define OG_KEY_LEFT_ALT        ::Orange::Key::LeftAlt
#define OG_KEY_LEFT_SUPER      ::Orange::Key::LeftSuper
#define OG_KEY_RIGHT_SHIFT     ::Orange::Key::RightShift
#define OG_KEY_RIGHT_CONTROL   ::Orange::Key::RightControl
#define OG_KEY_RIGHT_ALT       ::Orange::Key::RightAlt
#define OG_KEY_RIGHT_SUPER     ::Orange::Key::RightSuper
#define OG_KEY_MENU            ::Orange::Key::Menu