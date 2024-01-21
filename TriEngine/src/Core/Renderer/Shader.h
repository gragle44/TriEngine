#pragma once
#include <string>
#include <glm/glm.hpp>

namespace TriEngine {
	class Shader {
	public:
		virtual ~Shader() = default;

		static Shader* Create(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath);

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual const std::string& GetName() const = 0;

		virtual void SetInt(const std::string& name, int value) = 0;
		virtual void SetFloat(const std::string& name, float value) = 0;
		virtual void SetFloat2(const std::string& name, const glm::vec2& value) = 0;
		virtual void SetFloat3(const std::string& name, const glm::vec3& value) = 0;
		virtual void SetFloat4(const std::string& name, const glm::vec4& value) = 0;
		virtual void SetMat3(const std::string& name, const glm::mat3& value) = 0;
		virtual void SetMat4(const std::string& name, const glm::mat4& value) = 0;
		virtual void SetBool(const std::string& name, bool value) = 0;
	};
}