#pragma once

#include "Core/Base/Core.h"
#include "Core/GameObjects/Scene.h"

namespace TriEngine {
	class DebugModule {
	public:
		DebugModule() = default;

		void OnUpdate(float deltaTime);
		void OnImGuiRender();
	private:
		float m_FrameTime;
	};
}
