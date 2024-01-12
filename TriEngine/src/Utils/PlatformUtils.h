#pragma once

#include <string>

namespace TriEngine {
	class Time {
	public:
		static float GetTime();
	};

	class FileManager {
	public:
		static std::string ReadFromFile(const std::string& filter);
	};
}
