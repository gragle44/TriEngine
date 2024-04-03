#include "FileMenu.h"

#include "Core/Base/Application.h"
#include <imgui.h>

namespace TriEngine {
	void FileMenu::OnImGuiRender()
	{
		RenderFile();
		RenderProject();
	}

	void FileMenu::RenderFile()
	{
		
	}

	void FileMenu::RenderProject()
	{
		static bool showSettings = false;
		if (ImGui::BeginMenu("Project")) {
			ImGui::MenuItem("Settings", nullptr, &showSettings);
			ImGui::EndMenu();
		}
		if (showSettings)
			RenderProjectSettings(&showSettings);
	}

	void FileMenu::RenderProjectSettings(bool* show)
	{
		static bool showDemo;

		if (show) {
			if (!ImGui::Begin("Settings", show)) {
				ImGui::End();
			}
			else {
				ImGui::Text("dnaslk");
				if (ImGui::Button("Show ImGui demo"))
					showDemo = true;

				ImGui::End();
			}
		}
		if (showDemo)
			ImGui::ShowDemoWindow(&showDemo);
	}
}