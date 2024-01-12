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
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}