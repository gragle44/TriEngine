#include "tripch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace TriEngine {
	std::unique_ptr<RendererAPI> RenderCommand::s_RendererAPI(new OpenGLRendererAPI());
}