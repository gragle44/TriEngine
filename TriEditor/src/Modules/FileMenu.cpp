#include "FileMenu.h"

#include "../EditorUtils.h"
#include "Core/Base/Application.h"
#include "magic_enum.hpp"
#include <imgui.h>

namespace TriEngine {
	FileMenu::FileMenu(EditorData* data)
		:m_Data(data) {}

	void FileMenu::OnImGuiRender()
	{
		RenderProject();
	}
	
	//////////////
	// Project tab
	//////////////
	void FileMenu::RenderProject()
	{
		static bool showSettings = false;
		static bool showResourceViewer = false;

		if (ImGui::BeginMenu("Project")) {
			ImGui::MenuItem("Settings", nullptr, &showSettings);
			ImGui::MenuItem("Resource Viewer", nullptr, &showResourceViewer);
			ImGui::EndMenu();
		}

		if (showSettings)
			RenderProjectSettings(&showSettings);

		if (showResourceViewer)
			RenderResourceViewer(&showResourceViewer);

	}

	// Project settings

	void FileMenu::RenderGeneralProjectSettings() {
		auto& projData = ProjectManager::GetCurrentProjectData();
		//TODO: project name

		bool startupSceneExists = ResourceManager::ResourceExists(projData.StartupSceneID);
		std::string_view sceneName = "Unset";
		if (startupSceneExists)
			sceneName = std::dynamic_pointer_cast<Scene>(ResourceManager::Get(projData.StartupSceneID))->GetName();

		ImGui::Text("Startup Scene: %s", sceneName.data());
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_ForTooltip)) {
			std::string sceneHoverText = "Unknown";
			if (startupSceneExists) {
				ResourceID id = std::dynamic_pointer_cast<Scene>(ResourceManager::Get(projData.StartupSceneID))->MetaData.ID;
				sceneHoverText = std::to_string(id);
			}
			ImGui::SetTooltip("Resource ID: %s", sceneHoverText.c_str());
		}
		ImGui::SameLine();
		if (ImGui::ImageButton("general_settings_folder", (ImTextureID)m_Data->FolderTexture->GetID(), { 16.0f, 16.0f }, { 0, 1 }, { 1, 0 })) {
			std::string newPath = OpenFileDialog(ProjectManager::GetCurrent()->GetWorkingDirectory().generic_string(), ".tscn").generic_string();
			ResourceID id = ResourceManager::GetIDFromPath(newPath);

			if (ResourceManager::ResourceExists(id)) {
				projData.StartupSceneID = id;
			}
		}
		
	}

	static void RenderWindowProjectSettings() {
		auto& windowSettings = ProjectManager::GetCurrentProjectData().WindowSettings;

		ImGui::InputInt("Width", (int32_t*)&windowSettings.Width, 1, 100);
		ImGui::InputInt("Height", (int32_t*)&windowSettings.Height);
		ImGui::Checkbox("Resizable", &windowSettings.Resizable);
	}

	void FileMenu::RenderProjectSettings(bool* show)
	{
		static bool showDemo = false;

		static constexpr std::array<std::string_view, 2> categories = {"General", "Window"};

		if (ImGui::Begin("Settings", show)) {
				static int32_t selected = 0;

				if (ImGui::Button("Show ImGui demo"))
					showDemo = true;
				{
					ImGui::BeginChild("settings left pane", ImVec2(150, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX);
					for (int32_t i = 0; i < categories.size(); i++)
					{
						if (ImGui::Selectable(categories[i].data(), selected == i))
							selected = i;
					}
					ImGui::EndChild();
				}
        		ImGui::SameLine();

				{
					ImGui::BeginChild("settings right pane", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));

					float windowWidth = ImGui::GetWindowSize().x;
					float textWidth = ImGui::CalcTextSize(categories[selected].data()).x;

					ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
					ImGui::Text(categories[selected].data());

					switch (selected)
					{
					case 0:
						RenderGeneralProjectSettings();
						break;
					case 1:
						RenderWindowProjectSettings();
						break;
					
					default: TRI_CORE_ASSERT(false, "No settings page exists for selected tab");
					}

					ImGui::EndChild();
				}
        		ImGui::SameLine();

				ImGui::End();
			}

		if (showDemo)
			ImGui::ShowDemoWindow(&showDemo);
	}

	// Resource viewer

	void FileMenu::RenderResourceViewer(bool* show)
	{
		static bool editingResource;
		static uint64_t editingResourceID;

		if (ImGui::Begin("Resource Viewer", show, ImGuiWindowFlags_AlwaysAutoResize)) {
			if (ImGui::BeginTable("resourcetable", 5, ImGuiTableFlags_SizingFixedFit))
			{
				// Submit columns name with TableSetupColumn() and call TableHeadersRow() to create a row with a header in each column.
				// (Later we will show how TableSetupColumn() has other uses, optional flags, sizing weight etc.)
				ImGui::TableSetupColumn("ID");
				ImGui::TableSetupColumn("Type");
				ImGui::TableSetupColumn("Filepath");
				ImGui::TableSetupColumn("Loaded");
				ImGui::TableSetupColumn("");
				ImGui::TableHeadersRow();

				const auto& resourceRegistry = ResourceManager::GetResourceRegistry();

				auto registryBegin = resourceRegistry.begin();

				for (auto row = resourceRegistry.begin(); row != resourceRegistry.end(); row++)
				{
					// Required so buttons work for each row
					ImGui::PushID(row->first);

					ImGui::TableNextRow();

					ImGui::TableSetColumnIndex(0);
					ImGui::Text("%llu", row->first);

					ImGui::TableSetColumnIndex(1);
					ImGui::Text(magic_enum::enum_name(row->second.Type).data());

					ImGui::TableSetColumnIndex(2);
					ImGui::Text(row->second.Filepath.c_str());
					if (ImGui::IsItemHovered(ImGuiHoveredFlags_ForTooltip))
						ImGui::SetTooltip(row->second.Filepath.c_str());

					ImGui::TableSetColumnIndex(3);
					ImGui::Text(ResourceManager::ResourceLoaded(row->first) ? "True" : "False");
					
					ImGui::TableSetColumnIndex(4);
					if (ImGui::SmallButton("Edit")) {
						editingResource = true;
						editingResourceID = row->first;
					}

					ImGui::PopID();

				}
				ImGui::EndTable();
			}
			ImGui::End();
		}

		RenderResourceEditor(&editingResource, editingResourceID);
	}

	void FileMenu::RenderResourceEditor(bool* show, uint64_t& id)
	{
		if (id != 0 && *show)
			if (ImGui::Begin("Editing Resource", show, ImGuiWindowFlags_AlwaysAutoResize)) {

				auto& resourceMeta = ResourceManager::GetMetadata(id);

				std::string stringID = std::format("ID: {}", id);

				float windowWidth = ImGui::GetWindowSize().x;
				float textWidth = ImGui::CalcTextSize(stringID.c_str()).x;

				ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
				ImGui::Text(stringID.c_str());

				ImGui::Text(("Path: " + resourceMeta.Filepath).c_str());
				ImGui::SameLine();
				if (ImGui::ImageButton("resource_editor_folder", (ImTextureID)m_Data->FolderTexture->GetID(), { 16.0f, 16.0f }, { 0, 1 }, { 1, 0 })) {
					std::string newPath = OpenFileDialog(ProjectManager::GetCurrent()->GetWorkingDirectory().generic_string(), Utils::ExtensionFromResourceType(resourceMeta.Type)).generic_string();
				}

				if (ImGui::Button("Remove")) {
					ResourceManager::Remove(id);
					id = 0;
				}
				ImGui::SameLine();
				if (ImGui::Button("Reload")) {
					ResourceManager::Load(resourceMeta);
				}

				ImGui::End();
			}
	}
}