#include "SceneModule.h"

#include "Core/GameObjects/Components.h"
#include "Core/GameObjects/Script.h"
#include "Core/Renderer/Texture.h"
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

namespace TriEngine {
	static void HelpMarker(const char* desc, bool sameLine = false)
	{
		if (sameLine)
			ImGui::SameLine();
		ImGui::TextDisabled("(?)");
		if (ImGui::BeginItemTooltip())
		{
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted(desc);
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
	}

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
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		bool expanded = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)object.GetHandle(), flags, tag.Tag.c_str());
		if (ImGui::IsItemClicked()) {
			m_SelectedItem = object;
		}

		if (expanded)
			ImGui::TreePop();
	}

	template<typename T, typename Function>
	void TriEngine::SceneModule::DrawComponent(const std::string& name, GameObject object, Function function)
	{
		//TODO: Make this work
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

		if (object.HasComponent<Sprite2DComponent>()) {
			if (ImGui::TreeNodeEx((void*)typeid(Sprite2DComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Sprite2D")) {
				auto& sprite = object.GetComponent<Sprite2DComponent>();
				ImGui::ShowDemoWindow();


				ImDrawList* draw_list = ImGui::GetWindowDrawList();
				ImVec2 p0 = ImGui::GetCursorScreenPos();

				draw_list->AddImage((void*)sprite.Texture->GetID(), p0, ImVec2(p0.x + 250.0f, p0.y + 250.0f), ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
				//TODO: Draw checkerboard

				//ImGui::Image((void*)sprite.Texture->GetID(), { (float)sprite.Texture->GetWidth(), (float)sprite.Texture->GetHeight() }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 }, ImVec4(1, 1, 1, 1), ImVec4(0.2352941185235977f, 0.2156862765550613f, 0.5960784554481506f, 1.0f));
				//ImGui::Image((void*)sprite.Texture->GetID(), { (float)sprite.Texture->GetWidth(), (float)sprite.Texture->GetHeight() }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 }, ImVec4(1, 1, 1, 1), ImVec4(0.2352941185235977f, 0.2156862765550613f, 0.5960784554481506f, 1.0f));
				if (ImGui::BeginItemTooltip()) {

					static const std::string dimensionsBase = "Dimensions: ";
					std::stringstream dss;
					dss << '(' << sprite.Texture->GetWidth() << ", " << sprite.Texture->GetHeight() << ')';
					std::string dimensions = dimensionsBase + dss.str();
					ImGui::Text(dimensions.c_str());

					static const std::string filterBase = "Filter Mode: ";
					std::string filterStr = TextureFilterToString(sprite.Texture->GetFilterMode());
					std::string filterMode = filterBase + filterStr;
					ImGui::Text(filterMode.c_str());

					static const std::string wrapBase = "Wrap Mode: ";
					std::string wrapStr = TextureWrapToString(sprite.Texture->GetWrapMode());
					std::string wrapMode = wrapBase + wrapStr;
					ImGui::Text(wrapMode.c_str());

					ImGui::EndTooltip();
				}
				ImGui::ColorEdit4("Tint", glm::value_ptr(sprite.Tint));
				ImGui::DragFloat("Tiling Factor", &sprite.TilingFactor);

				ImGui::TreePop();
			}
		}

		if (object.HasComponent<Camera2DComponent>()) {
			if (ImGui::TreeNodeEx((void*)typeid(Camera2DComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera2D")) {
				auto& camera = object.GetComponent<Camera2DComponent>();

				ImGui::Checkbox("Resizeable", &camera.Resizeable); HelpMarker("Determines if the camera's aspect ratio can be changed.", true);

				static float cameraSize = camera.Camera.m_Zoom;
				if (ImGui::DragFloat("Zoom", &cameraSize, 1.0f, 0.1f, 100.0f)) {
					camera.Camera.SetSize(cameraSize, camera.Camera.m_NearClip, camera.Camera.m_FarClip);
				} 
				HelpMarker("Adjust the magnification level of the orthographic camera, controlling how much of the scene is visible within the viewport. Lower values correspond to more zoom.", true);

				if (ImGui::DragFloat2("Clipping", &camera.Camera.m_NearClip))
					camera.Camera.RecalculateProjection();
				HelpMarker("Set the range of visibility for objects in the scene, with the Near Plane defining the closest distance and the Far Plane defining the farthest distance before objects will be culled.", true);

				ImGui::TreePop();
			}
		}

		if (object.HasComponent<ScriptComponent>()) {
			if (ImGui::TreeNodeEx((void*)typeid(ScriptComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Scripts")) {
				auto& script = object.GetComponent<ScriptComponent>();

				std::string scriptName = typeid(*(script.ScriptInstance)).name();
				// Remove "class " from the start of the name
				scriptName.erase(0, 6);

				ImGui::Checkbox(scriptName.c_str(), &script.ScriptActive);
				ImGui::TreePop();
			}
		}
	}
}