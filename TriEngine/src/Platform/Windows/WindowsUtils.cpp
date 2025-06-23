#include "tripch.h"
#include "Utils/PlatformUtils.h"

#include <GLFW/glfw3.h>
#include <chrono>

#include <filesystem>

namespace TriEngine {
	float Time::GetTime() {
		return (float)glfwGetTime();
	}

	std::string FileManager::ReadFromFile(const std::string& filePath) {
		std::filesystem::path currentPath = std::filesystem::current_path().parent_path() / "TriEngine";
		std::filesystem::path relativePath = filePath;
		std::filesystem::path fullPath = currentPath / relativePath;

		std::ifstream file(fullPath);

		if (!file.is_open()) {
			TRI_CORE_ERROR("Could not open file {0}!", fullPath);
			return "";
		}
		
		std::stringstream stream;
		stream << file.rdbuf();

		file.close();

		return stream.str();
	}
}
