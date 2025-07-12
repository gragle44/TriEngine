#include "tripch.h"
#include "OpenGLShader.h"
#include "Core/Renderer/Shader.h"

#include "Base/Assert.h"

#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

namespace TriEngine {
	OpenGLShader::OpenGLShader(std::string_view name, std::string_view vertexSource, std::string_view fragmentSource) 
		:m_Name(name)
	{
		uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);

		const char* source = vertexSource.data();

		glShaderSource(vertexShader, 1, &source, 0);
		glCompileShader(vertexShader);

		int maxLength, isCompiled;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);

		if (maxLength != 0) {
			std::vector<char> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

			if (!isCompiled) {
				glDeleteShader(vertexShader);

				TRI_CORE_ERROR("Vertex Shader compilation {0}", infoLog.data());
				TRI_CORE_ASSERT(false, "Vertex shader compilation failure!");
				return;
			}
			else 
				TRI_CORE_WARN("Vertex Shader compilation {0}", infoLog.data());
		}

		uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		source = fragmentSource.data();

		glShaderSource(fragmentShader, 1, &source, 0);
		glCompileShader(fragmentShader);

		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);

		if (maxLength != 0) {
			std::vector<char> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

			if (!isCompiled) {
				glDeleteShader(vertexShader);

				TRI_CORE_ERROR("Vertex Shader compilation {0}", infoLog.data());
				TRI_CORE_ASSERT(false, "Vertex shader compilation failure!");
				return;
			}
			else
				TRI_CORE_WARN("Vertex Shader compilation {0}", infoLog.data());
		}

		m_ShaderID = glCreateProgram();
		uint32_t program = m_ShaderID;

		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);

		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		int32_t isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int32_t*)&isLinked);
		if (!isLinked)
		{
			int32_t maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<char> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			glDeleteProgram(program);
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			TRI_CORE_ERROR("{0}", infoLog.data());
			TRI_CORE_ASSERT(false, "Shader link failure!");
			return;
		}

		glDetachShader(program, vertexShader);
		glDeleteShader(vertexShader);
		glDetachShader(program, fragmentShader);
		glDeleteShader(fragmentShader);
	}

	OpenGLShader::~OpenGLShader() {
		glDeleteProgram(m_ShaderID);
	}

	void OpenGLShader::Bind() const {
		glUseProgram(m_ShaderID);
	}

	void OpenGLShader::UnBind() const {
		glUseProgram(0);
	}

	void OpenGLShader::SetInt(const std::string& name, int value)
	{
		glProgramUniform1i(m_ShaderID, GetUniformLocation(name), value);
	}

	void OpenGLShader::SetIntArray(const std::string& name, int* value, uint32_t count)
	{
		glProgramUniform1iv(m_ShaderID, GetUniformLocation(name), count, value);
	}

	void OpenGLShader::SetFloat(const std::string& name, float value)
	{
		glProgramUniform1f(m_ShaderID, GetUniformLocation(name), value);
	}

	void OpenGLShader::SetFloat2(const std::string& name, const glm::vec2& value)
	{
		glProgramUniform2f(m_ShaderID, GetUniformLocation(name), value.x, value.y);
	}

	void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& value)
	{
		glProgramUniform3f(m_ShaderID, GetUniformLocation(name), value.x, value.y, value.z);
	}

	void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& value)
	{
		glProgramUniform4f(m_ShaderID, GetUniformLocation(name), value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::SetMat3(const std::string& name, const glm::mat3& value)
	{
		glProgramUniformMatrix3fv(m_ShaderID, GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
	}

	void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& value)
	{
		glProgramUniformMatrix4fv(m_ShaderID, GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
	}

	void OpenGLShader::SetBool(const std::string& name, bool value)
	{
		glProgramUniform1i(m_ShaderID, GetUniformLocation(name), (int)value);
	}

	int32_t OpenGLShader::GetUniformLocation(const std::string& name)
	{
		if (m_UniformLocationCache.contains(name))
			return m_UniformLocationCache.at(name);

		int32_t location = glGetUniformLocation(m_ShaderID, name.c_str());

		if (location == -1) {
			TRI_CORE_WARN("Could not find uniform {0}!", name);
		}

		m_UniformLocationCache[name] = location;

		return location;
	}

	OpenGLComputeShader::OpenGLComputeShader(std::string_view name, std::string_view source)
		:m_Name(name)
	{
		uint32_t compute = glCreateShader(GL_COMPUTE_SHADER);

		const char* tempSource = source.data();

		glShaderSource(compute, 1, &tempSource, 0);
		glCompileShader(compute);

		int32_t maxLength, isCompiled;
		glGetShaderiv(compute, GL_INFO_LOG_LENGTH, &maxLength);
		glGetShaderiv(compute, GL_COMPILE_STATUS, &isCompiled);

		if (maxLength != 0) {
			std::vector<char> infoLog(maxLength);
			glGetShaderInfoLog(compute, maxLength, &maxLength, &infoLog[0]);

			if (!isCompiled) {
				glDeleteShader(compute);

				TRI_CORE_ERROR("Compute Shader {0} compilation {1}", m_Name, infoLog.data());
				TRI_CORE_ASSERT(false, "compute shader compilation failure!");
				return;
			}
			else
				TRI_CORE_WARN("Compute Shader {0} compilation {1}", m_Name, infoLog.data());
		}

		m_ShaderID = glCreateProgram();
		glAttachShader(m_ShaderID, compute);
		glLinkProgram(m_ShaderID);

		int32_t isLinked = 0;
		glGetProgramiv(m_ShaderID, GL_LINK_STATUS, (int32_t*)&isLinked);
		if (!isLinked)
		{
			int32_t maxLength = 0;
			glGetProgramiv(m_ShaderID, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<char> infoLog(maxLength);
			glGetProgramInfoLog(m_ShaderID, maxLength, &maxLength, &infoLog[0]);

			glDeleteProgram(m_ShaderID);
			glDeleteShader(compute);

			TRI_CORE_ERROR("Compute Shader {0}: {1}", m_Name, infoLog.data());
			TRI_CORE_ASSERT(false, "Shader link failure!");
			return;
		}

		glDetachShader(m_ShaderID, compute);
		glDeleteShader(compute);
	}

	OpenGLComputeShader::~OpenGLComputeShader()
	{
		glDeleteProgram(m_ShaderID);
	}

	void OpenGLComputeShader::Bind() const {
		glUseProgram(m_ShaderID);
	}

	void OpenGLComputeShader::Dispatch(uint32_t groupsX, uint32_t groupsY, uint32_t groupsZ)
	{
		glDispatchCompute(groupsX, groupsY, groupsZ);

		//TODO: make this better
		//glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);	
	}

	int32_t OpenGLComputeShader::GetUniformLocation(const std::string& name)
	{
		if (m_UniformLocationCache.contains(name))
			return m_UniformLocationCache.at(name);

		int32_t location = glGetUniformLocation(m_ShaderID, name.c_str());

		if (location == -1) {
			TRI_CORE_WARN("Could not find uniform {0}!", name);
		}

		m_UniformLocationCache[name] = location;

		return location;
	}

	void OpenGLComputeShader::SetInt(const std::string& name, int value)
	{
		glProgramUniform1i(m_ShaderID, GetUniformLocation(name), value);
	}

	void OpenGLComputeShader::SetIntArray(const std::string& name, int* value, uint32_t count)
	{
		glProgramUniform1iv(m_ShaderID, GetUniformLocation(name), count, value);
	}

	void OpenGLComputeShader::SetFloat(const std::string& name, float value)
	{
		glProgramUniform1f(m_ShaderID, GetUniformLocation(name), value);
	}

	void OpenGLComputeShader::SetFloat2(const std::string& name, const glm::vec2& value)
	{
		glProgramUniform2f(m_ShaderID, GetUniformLocation(name), value.x, value.y);
	}

	void OpenGLComputeShader::SetFloat3(const std::string& name, const glm::vec3& value)
	{
		glProgramUniform3f(m_ShaderID, GetUniformLocation(name), value.x, value.y, value.z);
	}

	void OpenGLComputeShader::SetFloat4(const std::string& name, const glm::vec4& value)
	{
		glProgramUniform4f(m_ShaderID, GetUniformLocation(name), value.x, value.y, value.z, value.w);
	}

	void OpenGLComputeShader::SetMat3(const std::string& name, const glm::mat3& value)
	{
		glProgramUniformMatrix3fv(m_ShaderID, GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
	}

	void OpenGLComputeShader::SetMat4(const std::string& name, const glm::mat4& value)
	{
		glProgramUniformMatrix4fv(m_ShaderID, GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
	}

	void OpenGLComputeShader::SetBool(const std::string& name, bool value)
	{
		glProgramUniform1i(m_ShaderID, GetUniformLocation(name), (int)value);
	}
}