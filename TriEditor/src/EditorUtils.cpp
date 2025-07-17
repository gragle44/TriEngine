#include "EditorUtils.h"

#include "TriEngine.h"

#include "imgui.h"
#include "nfd.h"

std::filesystem::path OpenFileDialog(std::string_view initial_path, std::string_view filetype) {
	nfdchar_t* output;
	nfdresult_t result = NFD_OpenDialog(filetype.data(), initial_path.data(), &output);

	std::string path;

	if (result == NFD_OKAY) {
		path = output;
		free(output);
	}

	else if (result == NFD_CANCEL) {
		TRI_CORE_TRACE("Canceled file dialog");
	}

	else if (result == NFD_ERROR) {
		TRI_CORE_ERROR("Error opening file dialog: {0}", NFD_GetError());
	}

	return path;
}

std::filesystem::path SaveFileDialog(std::string_view initial_path, std::string_view filetype) {
	nfdchar_t* output;
	nfdresult_t result = NFD_SaveDialog(filetype.data(), initial_path.data(), &output);

	std::string path;

	if (result == NFD_OKAY) {
		path = output;
		free(output);
	}

	else if (result == NFD_CANCEL) {
		TRI_CORE_TRACE("Canceled file dialog");
	}

	else if (result == NFD_ERROR) {
		TRI_CORE_ERROR("Error opening file dialog: {0}", NFD_GetError());
	}

	return path;
}

void HelpMarker(std::string_view desc, bool sameLine)
{
    if (sameLine)
        ImGui::SameLine();
    ImGui::TextDisabled("(?)");
    if (ImGui::BeginItemTooltip())
    {
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc.data());
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}
