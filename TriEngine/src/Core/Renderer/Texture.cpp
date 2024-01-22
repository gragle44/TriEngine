#include "tripch.h"
#include "Texture.h"

#include "Base/Assert.h"

#include "Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace TriEngine {
	Texture2D* Texture2D::Create(const std::string& filePath)
	{
		switch (Renderer::GetCurrentAPI())
		{
		case RendererAPI::API::None:
			TRI_CORE_ASSERT(false, "Renderer type 'None' is not currently supported");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return new OpenGLTexture2D(filePath);

			TRI_CORE_ASSERT(false, "Invalid renderer type!");
			return nullptr;
		}
	}
}