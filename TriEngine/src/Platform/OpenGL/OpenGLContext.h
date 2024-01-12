#pragma once

#include "Renderer/RenderingContext.h"

struct GLFWwindow;

namespace TriEngine {
	class TRI_API OpenGLContext : public RenderingContext {
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;
	private:
		GLFWwindow* m_WindowHandle;
	};
}