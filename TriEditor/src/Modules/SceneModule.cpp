#include "SceneModule.h"

#include "Core/GameObjects/Components.h"
#include "Core/GameObjects/Script.h"
#include "Core/Renderer/Texture.h"
#include "Core/Base/Input.h"
#include <imgui.h>
#include <nfd.h>
#include <glm/gtc/type_ptr.hpp>
#include <filesystem>

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

		auto view = m_Scene->m_Registry.group<TagComponent>();

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
				RenderComponentSelection<ScriptComponent>("Script", &addingComponent);
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

				char oldName[64];
				memset(oldName, 0, sizeof(oldName));
				tag.Tag.copy(oldName, tag.Tag.length());

				char buffer[64];
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
		constexpr ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen;
		if (object.HasComponent<T>()) {
			bool shouldRemove = false;
			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), flags, name.c_str());
			ImGui::SameLine();

			if (ImGui::SmallButton("X")) {
				shouldRemove = true;
			}

			if (open) {
				auto& component = object.GetComponent<T>();
				function(component);
				ImGui::TreePop();
			}

			if (shouldRemove) {
				object.RemoveComponent<T>();
			}
		}
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

		DrawComponent<Transform2DComponent>("Transform2D", object, [](Transform2DComponent& component) 
		{
			ImGui::DragFloat3("Position", glm::value_ptr(component.Position), 0.25f);
			ImGui::DragFloat("Rotation", &component.Rotation);
			ImGui::DragFloat2("Scale", glm::value_ptr(component.Scale));
		});
		
		DrawComponent<Sprite2DComponent>("Sprite2D", object, [&](Sprite2DComponent& sprite)
		{
			ImDrawList* draw_list = ImGui::GetWindowDrawList();
			ImVec2 p0 = ImGui::GetCursorScreenPos();
			draw_list->AddImage((void*)m_SpriteBackground->GetID(), ImVec2(p0.x, p0.y), ImVec2(p0.x + 250.0f, p0.y + 250.0f), ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
			draw_list->AddImage((void*)sprite.Texture->GetID(), ImVec2(p0.x, p0.y), ImVec2(p0.x + 250.0f, p0.y + 250.0f), ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
			ImGui::Dummy(ImVec2(250.0f, 250.0f));
			if (ImGui::BeginItemTooltip()) {

				const std::string dimensionsBase = "Dimensions: ";
				std::stringstream dss;
				dss << '(' << sprite.Texture->GetWidth() << ", " << sprite.Texture->GetHeight() << ')';
				std::string dimensions = dimensionsBase + dss.str();
				ImGui::Text(dimensions.c_str());

				const std::string filterBase = "Filter Mode: ";
				std::string filterStr = TextureFilterToString(sprite.Texture->GetFilterMode());
				std::string filterMode = filterBase + filterStr;
				ImGui::Text(filterMode.c_str());

				const std::string wrapBase = "Wrap Mode: ";
				std::string wrapStr = TextureWrapToString(sprite.Texture->GetWrapMode());
				std::string wrapMode = wrapBase + wrapStr;
				ImGui::Text(wrapMode.c_str());

				ImGui::EndTooltip();
			}
			
			if (ImGui::Button("Change image path")) {
				char* output;
				auto cwd = std::filesystem::current_path();
				nfdresult_t result = NFD_OpenDialog("png, jpeg, jpg", cwd.string().c_str(), &output);


				if (result == NFD_OKAY) {
					sprite.Texture = TriEngine::Texture2D::Create(output);
					delete output;
				}

				else if (result == NFD_CANCEL) {
					TRI_CORE_TRACE("Canceled file dialog");
				}

				else if (result == NFD_ERROR) {
					TRI_CORE_ERROR("Error opening file dialog: {0}", NFD_GetError());
				}

			}

			ImGui::ColorEdit4("Tint", glm::value_ptr(sprite.Tint));
			ImGui::DragFloat("Tiling Factor", &sprite.TilingFactor);
		});

		DrawComponent<Camera2DComponent>("Camera2D", object, [](Camera2DComponent& camera)
		{
			ImGui::Checkbox("Resizeable", &camera.Resizeable); HelpMarker("Determines if the camera's aspect ratio can be changed.", true);

			static float cameraSize = camera.Camera.m_Zoom;
			if (ImGui::DragFloat("Zoom", &cameraSize, 1.0f, 0.1f, 100.0f)) {
				camera.Camera.SetSize(cameraSize, camera.Camera.m_NearClip, camera.Camera.m_FarClip);
			}
			HelpMarker("Adjust the magnification level of the orthographic camera, controlling how much of the scene is visible within the viewport. Lower values correspond to more zoom.", true);

			if (ImGui::DragFloat2("Clipping", &camera.Camera.m_NearClip))
				camera.Camera.RecalculateProjection();
			HelpMarker("Set the range of visibility for objects in the scene, with the Near Plane defining the closest distance and the Far Plane defining the farthest distance before objects will be culled.", true);
		});

		DrawComponent<ScriptComponent>("Script", object, [](ScriptComponent& script)
		{
			if (ImGui::BeginCombo("Select script", script.ScriptName.c_str())) {
				for (const auto& pair : ScriptRegistry::Registry()) {

					bool selected = pair.first == script.ScriptName;
					if (ImGui::Selectable(pair.first.c_str(), &selected)) {
						if (pair.first != script.ScriptName) {
							script.InstantiateScript = pair.second;
							script.ScriptName = pair.first;
							script.ScriptInstance.release();
						}
					}

					if (selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			ImGui::Checkbox("Enabled", &script.ScriptActive);
		});
	}
}