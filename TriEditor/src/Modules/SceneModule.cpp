#include "SceneModule.h"

#include "Core/GameObjects/Components.h"
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

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
			GameObject object{ entity, m_Scene.get() };
			DrawNode(object);
		}

		ImGui::End();

		if (ImGui::Begin("Property Inspector")) {
			if (m_SelectedItem)
				DrawComponents(m_SelectedItem);
			else {
				constexpr char message[] = "No selected object";
				float windowWidth = ImGui::GetWindowSize().x;
				float textWidth = ImGui::CalcTextSize(message).x;

				ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
				ImGui::Text(message);
			}

		}
		ImGui::End();
	}

	void SceneModule::DrawNode(GameObject& object)
	{
		auto& tag = object.GetComponent<TagComponent>();

		ImGuiTreeNodeFlags flags = ((m_SelectedItem == object) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool expanded = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)object.GetHandle(), flags, tag.Tag.c_str());
		if (ImGui::IsItemClicked()) {
			m_SelectedItem = object;
		}

		if (expanded)
			ImGui::TreePop();
	}
	void SceneModule::DrawComponents(GameObject& object)
	{
		if (object.HasComponent<TagComponent>()) {
			auto& tag = object.GetComponent<TagComponent>();

			float windowWidth = ImGui::GetWindowSize().x;
			float textWidth = ImGui::CalcTextSize(tag.Tag.c_str()).x;

			ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
			ImGui::Text(tag.Tag.c_str());
		}

		if (object.HasComponent<Transform2DComponent>()) {
			if (ImGui::TreeNodeEx((void*)typeid(Transform2DComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform2D")) {
				auto& transform = object.GetComponent<Transform2DComponent>();

				ImGui::DragFloat3("Position", glm::value_ptr(transform.Position), 0.25f);
				ImGui::DragFloat("Rotation", &transform.Rotation);
				ImGui::DragFloat2("Scale", glm::value_ptr(transform.Scale));

				ImGui::TreePop();
			}

			//TODO: add tranform editor
		}
		else if (object.HasComponent<TransformComponent>()) {
			auto& transform = object.GetComponent<TransformComponent>();

			//TODO: add 3D transforms
		}

		if (object.HasComponent<Camera2DComponent>()) {
			if (ImGui::TreeNodeEx((void*)typeid(Camera2DComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera2D")) {
				auto& camera = object.GetComponent<Camera2DComponent>();

				ImGui::Checkbox("Resizeable", &camera.Resizeable);
				ImGui::TreePop();

			}
		}
	}
}