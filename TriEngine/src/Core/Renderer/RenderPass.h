#pragma once

#include "Framebuffer.h"
#include "Texture.h"

//TODO: This will describe where the renderer should render to. If Target is null, it will render to the screen.
namespace TriEngine {
	struct Renderpass {
		Renderpass() = default;
		Renderpass(const Reference<FrameBuffer>& target)
			:Target(target) {}

		Reference<FrameBuffer> Target = nullptr;

		bool Clear = true;
		bool DepthTest = true;
	};
}