#include "SceneModule.h"

#include "Core/GameObjects/Components.h"
#include "Core/GameObjects/Script.h"
#include "Core/Renderer/Texture.h"
#include "Core/Base/Input.h"
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
		m_SpriteBackground = Texture2D::Create("assets/sprite2Dbg.png");
	}

	void SceneModule::SetScene(const Reference<Scene>& scene)
	{
		m_Scene = scene;
		m_SpriteBackground = Texture2D::Create("assets/sprite2Dbg.png");
	}

	void SceneModule::OnImGuiRender()
	{
		// SCENE VIEWER //
		ImGui::Begin("Scene Viewer");

		auto view = m_Scene->m_Registry.view<TagComponent>();

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered()) {
			m_SelectedItem = {};
		}

		if (ImGui::Button("Create Object")) {
			GameObject object = m_Scene->CreateGameObject("Object");
			m_SelectedItem = object;
		}

		for (auto entity : view) {
			GameObject object{ entity, m_Scene.get() };
			DrawNode(object);
		}

		if (Input::IsKeyPressed(TRI_KEY_DELETE))
			if (m_SelectedItem) {
				m_Scene->DeleteGameObject(m_SelectedItem);
				m_SelectedItem = {};
			}

		ImGui::End();
		//////////////////

		// PROPERTY INSPECTOR //
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
		////////////////////////
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

		static bool renaming;
		static bool deleting;
		static bool addingComponent;

		if (!addingComponent && !renaming && ImGui::BeginPopupContextItem((const char*)&object)) {
			auto& tag = object.GetComponent<TagComponent>();

			if (ImGui::MenuItem("Add Component", "Ctrl + A")) {
				m_RightSelectedItem = object;
				addingComponent = true;
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::MenuItem("Rename", "Ctrl + R")) {
				m_RightSelectedItem = object;
				renaming = true;
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::MenuItem("Delete", "Del")) {
				m_RightSelectedItem = object;
				deleting = true;
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		if (addingComponent && m_RightSelectedItem == object) {
			if (!ImGui::IsPopupOpen("Add Component"))
				ImGui::OpenPopup("Add Component");

			ImVec2 center = ImGui::GetMainViewport()->GetCenter();
			ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

			if (ImGui::BeginPopupModal("Add Component", &addingComponent)) {
				RenderComponentSelection<Transform2DComponent>("Transform2D", &addingComponent);
				RenderComponentSelection<Sprite2DComponent>("Sprite2D", &addingComponent);
				RenderComponentSelection<Camera2DComponent>("Camera2D", &addingComponent);

				ImGui::End();
			}
		}

		if (renaming && m_RightSelectedItem == object) {
			if (!ImGui::IsPopupOpen("Renaming..."))
				ImGui::OpenPopup("Renaming...");

			ImVec2 center = ImGui::GetMainViewport()->GetCenter();
			ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
			if (ImGui::BeginPopupModal("Renaming...", &renaming, ImGuiWindowFlags_AlwaysAutoResize)) {
				auto& tag = object.GetComponent<TagComponent>();

				char oldName[128];
				memset(oldName, 0, sizeof(oldName));
				tag.Tag.copy(oldName, tag.Tag.length());

				char buffer[128];
				memset(buffer, 0, sizeof(buffer));
				strncpy_s(buffer, sizeof(buffer), tag.Tag.c_str(), sizeof(buffer));

				ImGuiInputTextFlags flags = ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue;
				if (ImGui::InputText("##", buffer, sizeof(buffer), flags)) {
					tag.Tag = std::string(buffer);
					renaming = false;
					TRI_CORE_INFO("Renamed object {0} to {1}", oldName, tag.Tag);
				}
				ImGui::Text("Characters remaining: %i", sizeof(buffer) - strlen(buffer));
				ImGui::EndPopup();
			}
		}

		if (expanded)
			ImGui::TreePop();

		if (deleting && m_RightSelectedItem == object) {
			if (!ImGui::IsPopupOpen("Delete Object"))
				ImGui::OpenPopup("Delete Object");

			ImVec2 center = ImGui::GetMainViewport()->GetCenter();
			ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
			if (ImGui::BeginPopupModal("Delete Object", &deleting, ImGuiWindowFlags_AlwaysAutoResize)) {

				ImGui::Text("Are you sure you want to delete this object? This action cannot be undone.");
				if (ImGui::Button("Delete")) {
					m_Scene->DeleteGameObject(object);
					deleting = false;
				}
				ImGui::EndPopup();
			}
		}
	}

	template<typename T>
	void SceneModule::RenderComponentSelection(std::string_view name, bool* stayOpen)
	{
		if (ImGui::Button(name.data())) {
			m_RightSelectedItem.AddComponent<T>();
			ImGui::CloseCurrentPopup();
			*stayOpen = false;
		}
	}

	template<typename T, typename Function>
	void SceneModule::DrawComponent(const std::string& name, GameObject object, Function function)
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

		const ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen;

		if (object.HasComponent<Transform2DComponent>()) {
			if (ImGui::TreeNodeEx((void*)typeid(Transform2DComponent).hash_code(), flags, "Transform2D")) {
				auto& transform = object.GetComponent<Transform2DComponent>();

				ImGui::DragFloat3("Position", glm::value_ptr(transform.Position), 0.25f);
				ImGui::DragFloat("Rotation", &transform.Rotation);
				ImGui::DragFloat2("Scale", glm::value_ptr(transform.Scale));

				ImGui::TreePop();
			}
		}
		else if (object.HasComponent<TransformComponent>()) {
			auto& transform = object.GetComponent<TransformComponent>();

			//TODO: add 3D transforms
		}

		if (object.HasComponent<Sprite2DComponent>()) {
			if (ImGui::TreeNodeEx((void*)typeid(Sprite2DComponent).hash_code(), flags, "Sprite2D")) {
				auto& sprite = object.GetComponent<Sprite2DComponent>();
				
				ImDrawList* draw_list = ImGui::GetWindowDrawList();
				ImVec2 p0 = ImGui::GetCursorScreenPos();
				draw_list->AddImage((void*)m_SpriteBackground->GetID(), ImVec2(p0.x, p0.y), ImVec2(p0.x + 250.0f, p0.y + 250.0f), ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
				draw_list->AddImage((void*)sprite.Texture->GetID(), ImVec2(p0.x, p0.y), ImVec2(p0.x + 250.0f, p0.y + 250.0f), ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
				ImGui::Dummy(ImVec2(250.0f, 250.0f));
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
			if (ImGui::TreeNodeEx((void*)typeid(Camera2DComponent).hash_code(), flags, "Camera2D")) {
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
			if (ImGui::TreeNodeEx((void*)typeid(ScriptComponent).hash_code(), flags, "Scripts")) {
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