#pragma once

#include <string>
#include <unordered_set>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <any>

namespace Elaina
{
	enum class EUniformType;
	class CShaderProgram
	{
	public:
		enum class EShaderType
		{
			VERTEX = GL_VERTEX_SHADER,
			FRAGMENT = GL_FRAGMENT_SHADER,
			GEOMETRY = GL_GEOMETRY_SHADER,
		};

		CShaderProgram();
		~CShaderProgram();

		bool attachShader(const EShaderType& vShaderType, const std::string& vShaderPath);
		bool linkProgram();
		void use() const;
		void setUniform(const std::string& vName, int vValue) const;
		void setUniform(const std::string& vName, bool vValue) const;
		void setUniform(const std::string& vName, float vValue) const;
		void setUniform(const std::string& vName, const glm::vec2& vValue) const;
		void setUniform(const std::string& vName, const glm::vec3& vValue) const;
		void setUniform(const std::string& vName, const glm::vec4& vValue) const;
		void setUniform(const std::string& vName, const glm::mat4& vMat) const;
		void setUniform(const std::string& vName, const glm::mat3& vMat) const;
		const auto& getUniforms() const { return m_Uniforms; }

		static std::shared_ptr<CShaderProgram> createShaderProgram(const std::string& vVertPath, const std::string& vFragPath);

	private:
		static bool __dumpShaderCodeFromFile(const std::string& vShaderPath, EShaderType vShaderType, std::string& voShaderCode);
		static bool __compileShader(const std::string& vShaderCode, EShaderType vShaderType, GLuint& voShaderID);
		static bool __checkCompileError(GLuint vID, EShaderType vShaderType);
		static bool __checkLinkError(GLuint vID);
		static std::string __getShaderTypeName(EShaderType vShaderType);
		void __deleteShaderIDs();
		
		GLuint m_ProgramID;
		std::unordered_set<GLuint> m_ShaderIDs;
		std::unordered_map<std::string, EUniformType> m_Uniforms;
	};
}