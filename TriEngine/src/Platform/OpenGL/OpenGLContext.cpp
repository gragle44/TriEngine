#include "tripch.h"

#include "Base/Assert.h"

#include "OpenGLContext.h"

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

namespace TriEngine {
	static void glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam) {
        const char* _source;
        const char* _type;

        switch (source) {
        case GL_DEBUG_SOURCE_API:             _source = "API"; break;

        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   _source = "WINDOW SYSTEM"; break;

        case GL_DEBUG_SOURCE_SHADER_COMPILER: _source = "SHADER COMPILER"; break;

        case GL_DEBUG_SOURCE_THIRD_PARTY:     _source = "THIRD PARTY"; break;

        case GL_DEBUG_SOURCE_APPLICATION:     _source = "APPLICATION"; break;

        default: _source = "UNKNOWN"; break;
        }

        switch (type) {
        case GL_DEBUG_TYPE_ERROR: _type = "ERROR"; break;

        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: _type = "DEPRECATED BEHAVIOR"; break;

        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: _type = "UDEFINED BEHAVIOR"; break;

        case GL_DEBUG_TYPE_PORTABILITY: _type = "PORTABILITY"; break;

        case GL_DEBUG_TYPE_PERFORMANCE: _type = "PERFORMANCE"; break;

        case GL_DEBUG_TYPE_OTHER: _type = "OTHER"; break;

        case GL_DEBUG_TYPE_MARKER: _type = "MARKER"; break;

        default: _type = "UNKNOWN"; break;
        }

        switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:
            TRI_CORE_ERROR("OpenGL({1}) {0}: {2}", _type, _source, message);
            break;

        case GL_DEBUG_SEVERITY_MEDIUM:
            TRI_CORE_WARN("OpenGL({1}) {0}: {2}", _type, _source, message);
            break;

        case GL_DEBUG_SEVERITY_LOW:
            TRI_CORE_INFO("OpenGL({1}) {0}: {2}", _type, _source, message);
            break;

        case GL_DEBUG_SEVERITY_NOTIFICATION:
            TRI_CORE_TRACE("OpenGL({1}) {0}: {2}", _type, _source, message);
            break;
        }
	}

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

		TRI_CORE_INFO("OpenGL Info:");
		TRI_CORE_INFO("  Vendor: {0}", (const char*)glGetString(GL_VENDOR));
		TRI_CORE_INFO("  Rendering device: {0}", (const char*)glGetString(GL_RENDERER));
		TRI_CORE_INFO("  Version: {0}", (const char*)glGetString(GL_VERSION));

		#ifdef TRI_DEBUG_GL
			int context_flags = 0;
			glGetIntegerv(GL_CONTEXT_FLAGS, &context_flags);

			if (context_flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
				TRI_CORE_INFO("OpenGL Debug Context created!");
				glEnable(GL_DEBUG_OUTPUT);
				glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
				glDebugMessageCallback(glDebugOutput, nullptr);
				glDebugMessageControl(
					GL_DONT_CARE,
					GL_DONT_CARE,
					GL_DONT_CARE,
					0,
					nullptr,
					GL_TRUE
				);
			}
		#endif

		#ifdef TRI_ENABLE_ASSERTS
			TRI_CORE_ASSERT(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5), "Invalid OpenGL Version! 4.5 or greater is required.");
		#endif

	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}