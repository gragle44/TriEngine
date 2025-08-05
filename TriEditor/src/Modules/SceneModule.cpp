#include "SceneModule.h"

#include "Core/Resources/ResourceManager.h"
#include "Core/Projects/ProjectManager.h"
#include "Core/GameObjects/Components.h"
#include "Core/GameObjects/Script.h"
#include "Core/Renderer/Texture.h"
#include "Core/Base/Input.h"
#include "../EditorUtils.h"

#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"
#include "magic_enum.hpp"
#include "glm/gtc/type_ptr.hpp"

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

	void SceneModule::DuplicateObject(GameObject object) {
		GameObject duplicatedObject = m_Data->ActiveScene->DuplicateObject(object);
		m_Data->SelectedItem = duplicatedObject;
	}

	SceneModule::SceneModule(EditorData* data)
		:m_Data(data)
	{
		m_SpriteBackground = Texture2D::Create("assets/sprite2Dbg.png");
	}

	void SceneModule::OnImGuiRender()
	{
		// SCENE VIEWER //
		ImGui::Begin("Scene Viewer");

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered()) {
			m_Data->SelectedItem = {};
		}

		if (ImGui::Button("Create Object")) {
			//m_Data->CmdHistory.Add(new AddGameObjectCommand(m_Scene));

			GameObject object = m_Data->ActiveScene->CreateGameObject("Object");
			m_Data->SelectedItem = object;
		}

		auto view = m_Data->ActiveScene->m_Registry.view<TagComponent>();

		for (auto entity : view) {
			GameObject object{ entity, m_Data->ActiveScene.get() };
			if (object.HasComponent<RelationshipComponent>()) {
				const auto& relationship = object.GetComponent<RelationshipComponent>();

				if (relationship.Parent)
					continue;
			}
			DrawNode(object);
		}

		if (Input::IsKeyPressed(TRI_KEY_DELETE))
			if (m_Data->SelectedItem) {
				m_Data->ActiveScene->DeleteGameObject(m_Data->SelectedItem);
				m_Data->SelectedItem = {};
			}

		if (Input::IsKeyPressed(TRI_KEY_LEFT_CONTROL) || Input::IsKeyPressed(TRI_KEY_RIGHT_CONTROL)) {
			if (Input::IsKeyPressed(TRI_KEY_D))
				DuplicateObject(m_Data->SelectedItem);
		}

		ImGui::End();
		//////////////////

		// PROPERTY INSPECTOR // 
		if (ImGui::Begin("Property Inspector")) {
			if (m_Data->SelectedItem)
				DrawComponents(m_Data->SelectedItem);
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
		// Object ID 0 is reserved for a dummy object
		if (object.GetComponent<IDComponent>().ID == 0)
			return;
		auto& tag = object.GetComponent<TagComponent>();

		ImGuiTreeNodeFlags flags = ((m_Data->SelectedItem == object) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		bool expanded = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)object.GetHandle(), flags, tag.Tag.c_str());

		if (ImGui::IsItemClicked()) {
			m_Data->SelectedItem = object;
		}

		static bool renaming;
		static bool deleting;
		static bool addingComponent;

		if (!addingComponent && !renaming && ImGui::BeginPopupContextItem((const char*)&object)) {
			auto& tag = object.GetComponent<TagComponent>();

			if (ImGui::MenuItem("Add Component", "Ctrl + A")) {
				m_Data->RightSelectedItem = object;
				addingComponent = true;
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::MenuItem("Rename", "Ctrl + R")) {
				m_Data->RightSelectedItem = object;
				renaming = true;
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::MenuItem("Duplicate", "Ctrl + D")) {
				DuplicateObject(object);
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::MenuItem("Delete", "Del")) {
				m_Data->RightSelectedItem = object;
				deleting = true;
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::MenuItem("Add Child")) {
				GameObject newObject = m_Data->ActiveScene->CreateGameObject();
				m_Data->ActiveScene->AddChildToObject(object, newObject);
			}

			if (!object.HasParent()) {
				if (ImGui::MenuItem("Make prefab")) {
					Reference<Prefab> prefab = m_Data->ActiveScene->CreatePrefab(object);

					auto path = SaveFileDialog(ProjectManager::GetCurrent()->GetWorkingDirectory().generic_string(), "prefab");
	
					ResourceManager::Create<Prefab>(prefab, path);
				}
			}

			ImGui::EndPopup();
		}

		if (addingComponent && m_Data->RightSelectedItem == object) {
			if (!ImGui::IsPopupOpen("Add Component"))
				ImGui::OpenPopup("Add Component");

			ImVec2 center = ImGui::GetMainViewport()->GetCenter();
			ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

			if (ImGui::BeginPopupModal("Add Component", &addingComponent)) {
				//RenderComponentSelection<NativeScriptComponent>("NativeScript", object, &addingComponent);
				RenderComponentSelection<ScriptComponent>("Script", object, &addingComponent);
				RenderComponentSelection<Transform2DComponent>("Transform2D", object, &addingComponent);
				RenderComponentSelection<Sprite2DComponent>("Sprite2D", object, &addingComponent);
				RenderComponentSelection<RigidBody2DComponent>("RigidBody2D", object, &addingComponent);
				RenderComponentSelection<BoxCollider2DComponent>("BoxCollider2D", object, &addingComponent);
				RenderComponentSelection<Camera2DComponent>("Camera2D", object, &addingComponent);
				RenderComponentSelection<ParticleEmmiterComponent>("ParticleEmmitter", object, &addingComponent);

				ImGui::End();
			}
		}

		if (renaming && m_Data->RightSelectedItem == object) {
			if (!ImGui::IsPopupOpen("Renaming..."))
				ImGui::OpenPopup("Renaming...");

			ImVec2 center = ImGui::GetMainViewport()->GetCenter();
			ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
			if (ImGui::BeginPopupModal("Renaming...", &renaming, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove)) {
				auto& tag = object.GetComponent<TagComponent>();

				char oldName[64];
				memset(oldName, 0, sizeof(oldName));
				tag.Tag.copy(oldName, tag.Tag.length());

				char buffer[64];
				memset(buffer, 0, sizeof(buffer));
				strncpy(buffer, tag.Tag.c_str(), sizeof(buffer));

				ImGuiInputTextFlags flags = ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue;
				if (ImGui::InputText("##", buffer, sizeof(buffer), flags)) {
					tag.Tag = std::string(buffer);
					m_Data->ActiveScene->OnObjectRenamed(object, oldName, tag.Tag);
					renaming = false;
					TRI_CORE_TRACE("Renamed object {0} to {1}", oldName, tag.Tag);
				}
				ImGui::Text("Characters remaining: %i", sizeof(buffer) - strnlen(buffer, 64));
				ImGui::EndPopup();
			}
		}

		if (expanded) {
			if (object.HasComponent<RelationshipComponent>())
				for (UUID child : object.GetComponent<RelationshipComponent>().Children) {
					DrawNode(m_Data->ActiveScene->GetObjectByID(child));
				}
			ImGui::TreePop();
		}

		if (deleting && m_Data->RightSelectedItem == object) {
			if (!ImGui::IsPopupOpen("Delete Object"))
				ImGui::OpenPopup("Delete Object");

			ImVec2 center = ImGui::GetMainViewport()->GetCenter();
			ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
			if (ImGui::BeginPopupModal("Delete Object", &deleting, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove)) {

				ImGui::Text("Are you sure you want to delete this object? This action cannot be undone.");
				if (ImGui::Button("Delete") || Input::IsKeyPressed(TRI_KEY_ENTER)) {
					if (m_Data->SelectedItem == object)
						m_Data->SelectedItem = {};

					m_Data->RightSelectedItem = {};

					m_Data->ActiveScene->DeleteGameObject(object);

					deleting = false;
				}
				ImGui::EndPopup();
			}
		}
	}

	template<typename T>
	void SceneModule::RenderComponentSelection(std::string_view name, GameObject object, bool* stayOpen)
	{
		if (!object.HasComponent<T>())
			if (ImGui::Button(name.data())) {
				m_Data->RightSelectedItem.AddComponent<T>();
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

	static void AddSpriteTooltipText(const std::string& label, const std::string& text) {
		ImGui::Text((label + text).c_str());
	}

	void SceneModule::DrawComponents(GameObject& object)
	{
		if (object.HasComponent<TagComponent>()) {
			const auto& tag = object.GetComponent<TagComponent>();

			DrawCenteredText(tag.Tag);
		}

		DrawComponent<Transform2DComponent>("Transform2D", object, [](Transform2DComponent& component) 
		{
			ImGui::DragFloat3("Position", glm::value_ptr(component.Position), 0.25f);
			ImGui::DragFloat("Rotation", &component.Rotation);
			ImGui::DragFloat2("Scale", glm::value_ptr(component.Scale));
		});

		DrawComponent<ParticleEmmiterComponent>("ParticleEmmitter", object, [](ParticleEmmiterComponent& component)
			{
				ImGui::ColorEdit4("Min Color", glm::value_ptr(component.MinColor));
				ImGui::ColorEdit4("Max Color", glm::value_ptr(component.MaxColor));

				ImGui::DragFloat3("Min Offset", glm::value_ptr(component.MinOffset));
				ImGui::DragFloat3("Max Offset", glm::value_ptr(component.MaxOffset));

				ImGui::DragFloat2("Min Velocity", glm::value_ptr(component.MinVelocity));
				ImGui::DragFloat2("Max Velocity", glm::value_ptr(component.MaxVelocity));

				ImGui::DragFloat2("Min Accel", glm::value_ptr(component.MinAccel));
				ImGui::DragFloat2("Max Accel", glm::value_ptr(component.MaxAccel));

				ImGui::DragFloat("Min Life", &component.MinLife);
				ImGui::DragFloat("Max Life", &component.MaxLife);

				ImGui::DragFloat("Spawn Interval", &component.SpawnInterval);

				if (ImGui::Button("Texture")) {
					auto cwd = ProjectManager::GetCurrent()->GetWorkingDirectory();

					auto output = OpenFileDialog(cwd.generic_string(), "png");

					ResourceID resourceID = ResourceManager::GetIDFromPath(ResourceManager::GetRelativePath(output.string()));
					if (!TriEngine::ResourceManager::ResourceExists(resourceID))
						component.Texture = TriEngine::ResourceManager::Create<Texture2D>(output, output);
					else
						component.Texture = std::reinterpret_pointer_cast<Texture2D>(TriEngine::ResourceManager::Get(resourceID));
				}
			});
		
		DrawComponent<Sprite2DComponent>("Sprite2D", object, [&](Sprite2DComponent& sprite)
		{
			ImDrawList* draw_list = ImGui::GetWindowDrawList();
			ImVec2 p0 = ImGui::GetCursorScreenPos();

			const float bgSize = 250.0f;
			draw_list->AddImage((void*)(intptr_t)m_SpriteBackground->GetID(), ImVec2(p0.x, p0.y), ImVec2(p0.x + bgSize, p0.y + bgSize), ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

			float scaleFactor = std::min(bgSize / sprite.Texture->GetWidth(), bgSize / sprite.Texture->GetHeight());
			float posX = (sprite.Texture->GetWidth() * scaleFactor);
			float posY = (sprite.Texture->GetHeight() * scaleFactor);

			draw_list->AddImage((void*)(intptr_t)sprite.Texture->GetID(), ImVec2(p0.x, p0.y), ImVec2(p0.x + posX, p0.y + posY), ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
			ImGui::Dummy(ImVec2(250.0f, 250.0f));

			if (ImGui::BeginItemTooltip()) {
				AddSpriteTooltipText("Dimensions: ", "(" + std::to_string(sprite.Texture->GetWidth()) + ", " + std::to_string(sprite.Texture->GetHeight()) + ")");
				AddSpriteTooltipText("Filter Mode: ", TextureFilterToString(sprite.Texture->GetFilterMode()));
				AddSpriteTooltipText("Wrap Mode: ", TextureWrapToString(sprite.Texture->GetWrapMode()));
				AddSpriteTooltipText("Transparent: ", sprite.HasTransparency() ? "True" : "False");
				ImGui::EndTooltip();
			}
			
			if (ImGui::Button("Change image path")) {
				auto cwd = ProjectManager::GetCurrent()->GetWorkingDirectory();

				auto output = OpenFileDialog(cwd.string(), "png");

                ResourceID resourceID = ResourceManager::GetIDFromPath(ResourceManager::GetRelativePath(output.string()));
                if (!ResourceManager::ResourceExists(resourceID))
                    sprite.Texture = ResourceManager::Create<Texture2D>(output, output);
				else
					sprite.Texture = std::reinterpret_pointer_cast<Texture2D>(ResourceManager::Get(resourceID));
			}

			ImGui::ColorEdit4("Tint", glm::value_ptr(sprite.Tint));
			ImGui::DragFloat("Tiling Factor", &sprite.TilingFactor);
		});

		DrawComponent<RigidBody2DComponent>("RigidBody2D", object, [](RigidBody2DComponent& component)
			{
				constexpr const char* bodyTypes[] = { "None", "Static", "Dynamic", "Kinematic" };
				const char* currentType = bodyTypes[(uint8_t)component.Type];

				if (ImGui::BeginCombo("Type", currentType)) {
					for (int i = 1; i < 4; i++) {
						bool selected = currentType == bodyTypes[i];
						if (ImGui::Selectable(bodyTypes[i], &selected)) {
							currentType = bodyTypes[i];
							component.Type = (RigidBody2DComponent::BodyType)i;
						}

						if (selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}
			});

		DrawComponent<BoxCollider2DComponent>("BoxCollider2D", object, [](BoxCollider2DComponent& component)
			{
				ImGui::DragFloat2("Size", glm::value_ptr(component.Size), 0.25f);
				ImGui::DragFloat("Density", &component.Density);
				ImGui::DragFloat("Friction", &component.Friction);
				ImGui::DragFloat("Restitution", &component.Restitution);
				ImGui::DragFloat("Restitution Threshold", &component.RestitutionThreshold);
			});

		DrawComponent<Camera2DComponent>("Camera2D", object, [](Camera2DComponent& camera)
		{
				ImGui::Checkbox("Resizable", &camera.Resizable); HelpMarker("Determines if the camera's aspect ratio can be changed.", true);
				ImGui::Checkbox("Primary", &camera.Primary); HelpMarker("Sets the camera as the main scene camera.", true);

			static float cameraSize = camera.Camera.m_Zoom;
			if (ImGui::DragFloat("Zoom", &cameraSize, 1.0f, 0.1f, 100.0f)) {
				camera.Camera.SetSize(cameraSize, camera.Camera.m_NearClip, camera.Camera.m_FarClip);
			}
			HelpMarker("Adjust the magnification level of the orthographic camera, controlling how much of the scene is visible within the viewport. Lower values correspond to more zoom.", true);

			if (ImGui::DragFloat("Aspect Ratio", &camera.Camera.m_AspectRatio))
				camera.Camera.RecalculateProjection();

			if (ImGui::DragFloat2("Clipping", &camera.Camera.m_NearClip))
				camera.Camera.RecalculateProjection();
			HelpMarker("Set the range of visibility for objects in the scene, with the Near Plane defining the closest distance and the Far Plane defining the farthest distance before objects will be culled.", true);
		});

		DrawComponent<ScriptComponent>("Script", object, [object, this](ScriptComponent& script)
		{
			ScriptEngine& engine = ScriptEngine::Get();

			if (!script.Instance && !m_Data->SceneRunning && script.ScriptResource) {
				engine.InstantiateScript(object);
			}

			std::string scriptName = "No Script Selected";
			if (script.ScriptResource)
				scriptName = script.ScriptResource->Name;

			float size = ImGui::CalcItemWidth() / 1.5f;
			ImGui::PushItemWidth(size);
            ImGui::InputText("##", &scriptName, ImGuiInputTextFlags_ReadOnly);
            ImGui::PopItemWidth();

			ImGui::SameLine();
			if (ImGui::ImageButton("change script button", (ImTextureID)m_Data->FolderTexture->GetID(), { 16.0f, 16.0f }, { 0, 1 }, { 1, 0 })) {
				auto cwd = ProjectManager::GetCurrent()->GetWorkingDirectory();
				auto output = OpenFileDialog(cwd.string(), "as");

				if (!output.empty()) {
                    ResourceID resourceID = ResourceManager::GetIDFromPath(ResourceManager::GetRelativePath(output.string()));
                    if (!ResourceManager::ResourceExists(resourceID))
						script.ScriptResource = ResourceManager::Create<Script>(output.string());
					else
						script.ScriptResource = std::dynamic_pointer_cast<Script>(ResourceManager::Get(resourceID));
				}
			}

			ImGui::SameLine();
			
			if (ImGui::ImageButton("reload script button", (ImTextureID)m_Data->ReloadTexture->GetID(), { 16.0f, 16.0f }, { 0, 1 }, { 1, 0 })) {\
				engine.ClearAllScriptInstances(m_Data->ActiveScene.get());
				engine.BuildScript(script.ScriptResource);
				engine.InstantiateScript(object);
			}

			ImGui::Checkbox("Enabled", &script.Active);
			HelpMarker("Whether the script should be updated and recieve events.", true);

			// The script couldn't be instantiated
			if (!script.Instance)
				return;

			if (!script.Instance->Properties.empty()) {
				ImGui::Spacing();
				DrawCenteredText("Script Properties");
				ImGui::Spacing();
			}

			if (script.Instance) {
				for (const auto& [name, property] : script.Instance->Properties) {
					if (property.Type == ScriptVariableType::Bool) {
						ImGui::Checkbox(name.c_str(), reinterpret_cast<bool*>(property.Address));
						if (ImGui::IsItemHovered()) {
							ImGui::SetTooltip(magic_enum::enum_name(property.Type).data());
						}
					}
					else if (Utils::IsScriptVariableTypeScalar(property.Type)) {
						float size = ImGui::CalcItemWidth() / 1.75f;
						ImGui::PushItemWidth(size);
						ImGui::InputScalar(name.c_str(), ScriptScalarDataTypeToImGuiDataType(property.Type), property.Address, nullptr, nullptr, nullptr, ImGuiInputTextFlags_ElideLeft);
						if (ImGui::IsItemHovered()) {
							ImGui::SetTooltip(magic_enum::enum_name(property.Type).data());
						}
						ImGui::PopItemWidth();
					}
					else if (property.Type == ScriptVariableType::Vec2) {
						float size = ImGui::CalcItemWidth() / 1.75f;
						ImGui::PushItemWidth(size);
						ImGui::InputFloat2(name.c_str(), &reinterpret_cast<glm::vec2*>(property.Address)->x);
						if (ImGui::IsItemHovered()) {
							ImGui::SetTooltip(magic_enum::enum_name(property.Type).data());
						}
						ImGui::PopItemWidth();
					}
					else if (property.Type == ScriptVariableType::Vec3) {
						float size = ImGui::CalcItemWidth() / 1.75f;
						ImGui::PushItemWidth(size);
						ImGui::InputFloat3(name.c_str(), &reinterpret_cast<glm::vec3*>(property.Address)->x);
						if (ImGui::IsItemHovered()) {
							ImGui::SetTooltip(magic_enum::enum_name(property.Type).data());
						}
						ImGui::PopItemWidth();
					}
					else if (property.Type == ScriptVariableType::Vec4) {
						float size = ImGui::CalcItemWidth() / 1.75f;
						ImGui::PushItemWidth(size);
						ImGui::InputFloat4(name.c_str(), &reinterpret_cast<glm::vec4*>(property.Address)->x);
						if (ImGui::IsItemHovered()) {
							ImGui::SetTooltip(magic_enum::enum_name(property.Type).data());
						}
						ImGui::PopItemWidth();
					}
					else if (property.Type == ScriptVariableType::String) {
						float size = ImGui::CalcItemWidth() / 1.75f;
						ImGui::PushItemWidth(size);
						std::string* text = reinterpret_cast<std::string*>(property.Address);
						ImGui::InputText(name.c_str(), text);
						if (ImGui::IsItemHovered()) {
							ImGui::SetTooltip(magic_enum::enum_name(property.Type).data());
						}
						ImGui::PopItemWidth();
					}
				}
			}
		});

		DrawComponent<NativeScriptComponent>("NativeScript", object, [](NativeScriptComponent& script)
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