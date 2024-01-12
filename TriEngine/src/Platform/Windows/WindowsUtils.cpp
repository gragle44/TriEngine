#pragma once

#include "tripch.h"
#include "tripch.h"
#include "Utils/PlatformUtils.h"

#include <GLFW/glfw3.h>

#include <fstream>
#include <sstream>


namespace TriEngine {
	float Time::GetTime() {
		return (float)glfwGetTime();
	}

	std::string FileManager::ReadFromFile(const std::string& filePath) {
		std::ifstream file(filePath.c_str());

		TRI_CORE_ASSERT(file.is_open(), "Could not open specified file!", filePath)

		std::stringstream stream;
		stream << file.rdbuf();

		file.close();

		return stream.str();
	}
}
