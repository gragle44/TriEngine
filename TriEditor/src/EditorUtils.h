#pragma once

#include "TriEngine.h"

#include "imgui.h"

#include <filesystem>

std::filesystem::path OpenFileDialog(std::string_view initial_path, std::string_view filetype);
std::filesystem::path SaveFileDialog(std::string_view initial_path, std::string_view filetype);

ImGuiDataType ScriptScalarDataTypeToImGuiDataType(TriEngine::ScriptVariableType type);

void HelpMarker(std::string_view desc, bool sameLine = true);

void DrawCenteredText(std::string_view text);