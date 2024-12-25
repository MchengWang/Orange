#pragma once

#include "Orange/Window.h"

#include <GLFW/glfw3.h>

namespace Orange
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return o_Data.Width; }
		inline unsigned int GetHeight() const override { return o_Data.Height; }

		// ¥∞ø⁄ Ù–‘
		inline void SetEventCallback(const EventCallbackFn& callback) override { o_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

	private:
		GLFWwindow* o_Window;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData o_Data;
	};
}
