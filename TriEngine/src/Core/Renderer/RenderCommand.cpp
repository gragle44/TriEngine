#include "tripch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace TriEngine {
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI();
}