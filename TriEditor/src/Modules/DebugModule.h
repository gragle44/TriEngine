#pragma once

#include "../EditorData.h"

#include "Core/Base/Core.h"
#include "Core/GameObjects/Scene.h"

namespace TriEngine {
	class DebugModule {
	public:
		DebugModule(EditorData* data)
			:m_Data(data) {}

		void OnUpdate(float deltaTime);
		void OnImGuiRender();
	private:
		EditorData* m_Data;
		float m_FrameTime;
	};
}
