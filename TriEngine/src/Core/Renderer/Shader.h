#pragma once
#include <string>

namespace TriEngine {
	class Shader {
	public:
		Shader(const std::string& vertSource, const std::string& fragSource);
		~Shader();

		void Bind() const;
		void UnBind() const;
	private:
		uint32_t m_ShaderID;
	};
}