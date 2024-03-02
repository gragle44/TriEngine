#include "FileMenu.h"

#include "Core/Base/Application.h"
#include <imgui.h>

namespace TriEngine {
	void FileMenu::OnImGuiRender()
	{
		if (ImGui::BeginMenuBar())
		{

			RenderFile();
			RenderProject();

			ImGui::EndMenuBar();
		}
	}

	void FileMenu::RenderFile()
	{
		if (ImGui::BeginMenu("File"))
		{
			// Disabling fullscreen would allow the window to be moved to the front of other windows, 
			// which we can't undo at the moment without finer window depth/z control.
			//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

			if (ImGui::MenuItem("Exit")) Application::Get().Close();
			ImGui::EndMenu();
		}
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