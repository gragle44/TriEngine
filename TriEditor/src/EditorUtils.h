#pragma once

#include <filesystem>

std::filesystem::path OpenFileDialog(std::string_view initial_path, const char* filetype);
std::filesystem::path SaveFileDialog(std::string_view initial_path, const char* filetype);
