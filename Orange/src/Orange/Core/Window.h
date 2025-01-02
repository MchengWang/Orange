#pragma once

#include "Orange/Core/Core.h"
#include "Orange/Events/Event.h"

namespace Orange
{
	struct WindowProps
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps(const std::string& title = "Orange Enigne",
			                         unsigned int width = 1280,
			                         unsigned int height = 720)
					:Title(title), Width(width), Height(height) 
		{
		}
	};

	// 表示基于桌面系统的 Window 的界面
	class ORANGE_API Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {};

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		// 窗口属性
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	};								 
}