#include "FileMenu.h"

#include "Core/Base/Application.h"
#include <nfd.h>
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

	void FileMenu::RenderProjectSettings(bool* show)
	{
		static bool showDemo = false;

		if (ImGui::Begin("Settings", show)) {
				ImGui::Text("dnaslk");
				if (ImGui::Button("Show ImGui demo"))
					showDemo = true;

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
	
	static std::string OpenFileDialog(const std::string& extension) {
		char* output;
		std::string ret;
		auto cwd = ProjectManager::GetCurrent()->GetWorkingDirectory();
		nfdresult_t result = NFD_OpenDialog(extension.c_str(), cwd.string().c_str(), &output);


		if (result == NFD_OKAY) {
			ret = output;
			delete output;
		}

		else if (result == NFD_CANCEL) {
			TRI_CORE_TRACE("Canceled file dialog");
		}

		else if (result == NFD_ERROR) {
			TRI_CORE_ERROR("Error opening file dialog: {0}", NFD_GetError());
		}

		return ret;
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
					std::string newPath = OpenFileDialog(Utils::ExtensionFromResourceType(resourceMeta.Type));
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