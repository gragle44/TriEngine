#include "tripch.h"
#include "Core/Core.h"

#include "OpenGLContext.h"

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

namespace TriEngine {
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		:m_WindowHandle(windowHandle)
	{
		TRI_CORE_ASSERT(windowHandle, "Invalid GLFW Window handle!");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		TRI_CORE_ASSERT(status, "Failed to initialize Glad!");

		#ifdef TRI_ENABLE_ASSERTS
				int versionMajor;
				int versionMinor;
				glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
				glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

				TRI_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), "Invalid OpenGL Version! 4.5 or greater is required.");
		#endif
		
		TRI_CORE_INFO("OpenGL Info:");
		TRI_CORE_INFO("  Vendor: {0}", (const char*)glGetString(GL_VENDOR));
		TRI_CORE_INFO("  Rendering device: {0}", (const char*)glGetString(GL_RENDERER));
		TRI_CORE_INFO("  Version: {0}", (const char*)glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}