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

ImGuiDataType ScriptScalarDataTypeToImGuiDataType(TriEngine::ScriptVariableType type) {
	using namespace TriEngine;
	switch (type)
	{
	case ScriptVariableType::Int8:
		return ImGuiDataType_S8;
	case ScriptVariableType::Int16:
		return ImGuiDataType_S16;
	case ScriptVariableType::Int32:
		return ImGuiDataType_S32;
	case ScriptVariableType::Int64:
		return ImGuiDataType_S64;
	case ScriptVariableType::Uint8:
		return ImGuiDataType_U8;
	case ScriptVariableType::Uint16:
		return ImGuiDataType_U16;
	case ScriptVariableType::Uint32:
		return ImGuiDataType_U32;
	case ScriptVariableType::Uint64:
		return ImGuiDataType_U64;
	case ScriptVariableType::Float:
		return ImGuiDataType_Float;
	case ScriptVariableType::Double:
		return ImGuiDataType_Double;	
	default:
		TRI_CORE_ASSERT(false, "Invalid scalar type");
		return 0;
	}
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

void DrawCenteredText(std::string_view text) {
	float windowWidth = ImGui::GetWindowSize().x;
	float textWidth = ImGui::CalcTextSize(text.data()).x;

	ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
	ImGui::Text(text.data());
}
