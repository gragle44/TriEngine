#pragma once

#include <filesystem>

std::filesystem::path OpenFileDialog(std::string_view initial_path, std::string_view filetype);
std::filesystem::path SaveFileDialog(std::string_view initial_path, std::string_view filetype);

void HelpMarker(std::string_view desc, bool sameLine = true);