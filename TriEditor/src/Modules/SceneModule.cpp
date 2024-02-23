#include "SceneModule.h"

#include "Core/GameObjects/Components.h"
#include <imgui.h>

namespace TriEngine {
	SceneModule::SceneModule(const Reference<Scene>& scene)
		:m_Scene(scene)
	{
	}

	void SceneModule::SetScene(const Reference<Scene>& scene)
	{
		m_Scene = scene;
	}

	void SceneModule::OnImGuiRender()
	{
		ImGui::Begin("Scene Viewer");

		auto view = m_Scene->m_Registry.view<TagComponent>();

		for (auto entity : view) {
			auto& tag = view.get<TagComponent>(entity);
			ImGui::Text(tag.Tag.c_str());
		}

		ImGui::End();
	}
}