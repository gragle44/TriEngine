#pragma once

#include "Core/Window.h"
#include "Renderer/RenderingContext.h"

#include <GLFW/glfw3.h>

namespace TriEngine {

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline virtual unsigned int GetWidth() const final { return m_Data.Width; }
		inline virtual unsigned int GetHeight() const final { return m_Data.Height; }

		// Window attributes
		virtual inline void SetEventCallback(const EventCallbackFn& callback) final { m_Data.EventCallback = callback; }
		virtual void SetVSync(VsyncMode mode) final;
		virtual VsyncMode GetVSyncMode() const final;

		inline virtual void* GetNativeWindow() const final { return m_Window; }
	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
	private:
		GLFWwindow* m_Window;
		RenderingContext* m_Context;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			VsyncMode VSyncMode;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};

}