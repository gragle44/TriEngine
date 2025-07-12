#pragma once

#include <glm/glm.hpp>

#include <string>

namespace TriEngine {
	class Shader {
	public:
		virtual ~Shader() = default;

		static Reference<Shader> Create(std::string_view name, std::string_view vertexSource, std::string_view fragmentSource);

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual const std::string& GetName() const = 0;

		virtual void SetInt(const std::string& name, int value) = 0;
		virtual void SetIntArray(const std::string& name, int* value, uint32_t count) = 0;
		virtual void SetFloat(const std::string& name, float value) = 0;
		virtual void SetFloat2(const std::string& name, const glm::vec2& value) = 0;
		virtual void SetFloat3(const std::string& name, const glm::vec3& value) = 0;
		virtual void SetFloat4(const std::string& name, const glm::vec4& value) = 0;
		virtual void SetMat3(const std::string& name, const glm::mat3& value) = 0;
		virtual void SetMat4(const std::string& name, const glm::mat4& value) = 0;
		virtual void SetBool(const std::string& name, bool value) = 0;
	};

	class ComputeShader {
	public:
		virtual ~ComputeShader() = default;

		static Reference<ComputeShader> Create(std::string_view name, std::string_view source);

		virtual void Bind() const = 0;
		virtual void Dispatch(uint32_t groupsX, uint32_t groupsY, uint32_t groupsZ) = 0;

		virtual void SetInt(const std::string& name, int value) = 0;
		virtual void SetIntArray(const std::string& name, int* value, uint32_t count) = 0;
		virtual void SetFloat(const std::string& name, float value) = 0;
		virtual void SetFloat2(const std::string& name, const glm::vec2& value) = 0;
		virtual void SetFloat3(const std::string& name, const glm::vec3& value) = 0;
		virtual void SetFloat4(const std::string& name, const glm::vec4& value) = 0;
		virtual void SetMat3(const std::string& name, const glm::mat3& value) = 0;
		virtual void SetMat4(const std::string& name, const glm::mat4& value) = 0;
		virtual void SetBool(const std::string& name, bool value) = 0;
	};
}