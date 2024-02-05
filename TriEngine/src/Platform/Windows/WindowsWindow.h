#pragma once

#include "Core/Base/Window.h"
#include "Renderer/RenderingContext.h"

#include <GLFW/glfw3.h>

namespace TriEngine {

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		virtual uint32_t GetWidth() const final { return m_Data.Width; }
		virtual uint32_t GetHeight() const final { return m_Data.Height; }

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) final { m_Data.EventCallback = callback; }
		virtual void SetVSync(VsyncMode mode) final;
		virtual VsyncMode GetVSyncMode() const final;

		virtual void* GetNativeWindow() const final { return m_Window; }
	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
	private:
		GLFWwindow* m_Window;
		RenderingContext* m_Context;

		struct WindowData
		{
			std::string Title;
			uint32_t Width, Height;
			VsyncMode VSyncMode;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};

}