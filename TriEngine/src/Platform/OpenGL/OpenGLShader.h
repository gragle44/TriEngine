#pragma once

#include "Renderer/Shader.h"
#include <unordered_map>

namespace TriEngine {
	class OpenGLShader : public Shader {
	public:
		OpenGLShader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath);
		virtual ~OpenGLShader() final;

		virtual void Bind() const final;
		virtual void UnBind() const final;

		virtual const std::string& GetName() const final { return m_Name; }

		virtual void SetInt(const std::string& name, int value) final;
		virtual void SetIntArray(const std::string& name, int* value, uint32_t count) final;
		virtual void SetFloat(const std::string& name, float value) final;
		virtual void SetFloat2(const std::string& name, const glm::vec2& value) final;
		virtual void SetFloat3(const std::string& name, const glm::vec3& value) final;
		virtual void SetFloat4(const std::string& name, const glm::vec4& value) final;
		virtual void SetMat3(const std::string& name, const glm::mat3& value) final;
		virtual void SetMat4(const std::string& name, const glm::mat4& value) final;
		virtual void SetBool(const std::string& name, bool value) final;
	private:
		int GetUniformLocation(const std::string& name);

		std::string m_Name;
		uint32_t m_ShaderID;
		std::unordered_map<std::string, int> m_UniformLocationCache;
	};
}