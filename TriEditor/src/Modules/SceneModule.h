#pragma once

#include "Core/Base/Core.h"
#include "Core/GameObjects/Scene.h"

namespace TriEngine {
	class SceneModule {
	public:
		SceneModule() = default;
		SceneModule(const Reference<Scene>& scene);

		void SetScene(const Reference<Scene>& scene);

		void OnImGuiRender();
	private:
		Reference<Scene> m_Scene;
	};
}