#include "pch.h"
#include "ShaderProgram.h"
#include "parser/GlslParser.h"
#include "safe.h"

Elaina::CShaderProgram::CShaderProgram()
{
	m_ProgramID = glCreateProgram();
}

Elaina::CShaderProgram::~CShaderProgram()
{
	__deleteShaderIDs();
	GL_SAFE_CALL(glDeleteProgram(m_ProgramID));
}

bool Elaina::CShaderProgram::attachShader(const EShaderType& vShaderType, const std::string& vShaderPath)
{
	std::string ShaderCode;
	if (!__dumpShaderCodeFromFile(vShaderPath, vShaderType, ShaderCode))
	{
		return false;
	}

	GLuint ShaderID;
	if (!__compileShader(ShaderCode, vShaderType, ShaderID))
	{
		return false;
	}

	CGlslParser::parseUniforms(ShaderCode, m_Uniforms);

	GL_SAFE_CALL(glAttachShader(m_ProgramID, ShaderID));
	m_ShaderIDs.insert(ShaderID);
	return true;
}

bool Elaina::CShaderProgram::linkProgram()
{
	GL_SAFE_CALL(glLinkProgram(m_ProgramID));
	if (!__checkLinkError(m_ProgramID))
	{
		__deleteShaderIDs();
		GL_SAFE_CALL(glDeleteProgram(m_ProgramID));
		return false;
	}
	__deleteShaderIDs();
	return true;
}

void Elaina::CShaderProgram::autoUse() const
{
	GL_SAFE_CALL(glUseProgram(m_ProgramID));
	for (const auto& [Name, Uniform] : m_Uniforms) {
		const auto& [Type, Value] = Uniform;
		switch (Type)
		{
		case EUniformType::FLOAT:
			__setUniform(Name, std::any_cast<float>(Value));
			break;
		case EUniformType::VEC2:
			__setUniform(Name, std::any_cast<glm::vec2>(Value));
			break;
		case EUniformType::VEC3:
			__setUniform(Name, std::any_cast<glm::vec3>(Value));
			break;
		case EUniformType::VEC4:
			__setUniform(Name, std::any_cast<glm::vec4>(Value));
			break;
		case EUniformType::MAT3:
			__setUniform(Name, std::any_cast<glm::mat3>(Value));
			break;
		case EUniformType::MAT4:
			__setUniform(Name, std::any_cast<glm::mat4>(Value));
			break;
		case EUniformType::SAMPLER2D:
			__setUniform(Name, std::any_cast<int>(Value));
			break;
		default:
			spdlog::error("unsupported uniform type for: {}", Name);
		}
	}
}

void Elaina::CShaderProgram::__setUniform(const std::string& vName, int vValue) const
{
	GL_SAFE_CALL(glUniform1i(glGetUniformLocation(m_ProgramID, vName.c_str()), vValue));
}

void Elaina::CShaderProgram::__setUniform(const std::string& vName, bool vValue) const
{
	GL_SAFE_CALL(glUniform1i(glGetUniformLocation(m_ProgramID, vName.c_str()), (int)vValue));
}

void Elaina::CShaderProgram::__setUniform(const std::string& vName, float vValue) const
{
	GL_SAFE_CALL(glUniform1f(glGetUniformLocation(m_ProgramID, vName.c_str()), vValue));
}

void Elaina::CShaderProgram::__setUniform(const std::string& vName, const glm::vec2& vValue) const
{
	GL_SAFE_CALL(glUniform2fv(glGetUniformLocation(m_ProgramID, vName.c_str()), 1, &vValue[0]));
}

void Elaina::CShaderProgram::__setUniform(const std::string& vName, const glm::vec3& vValue) const
{
	GL_SAFE_CALL(glUniform3fv(glGetUniformLocation(m_ProgramID, vName.c_str()), 1, &vValue[0]));
}

void Elaina::CShaderProgram::__setUniform(const std::string& vName, const glm::vec4& vValue) const
{
	GL_SAFE_CALL(glUniform4fv(glGetUniformLocation(m_ProgramID, vName.c_str()), 1, &vValue[0]));
}

void Elaina::CShaderProgram::__setUniform(const std::string& vName, const glm::mat4& vMat) const
{
	GL_SAFE_CALL(glUniformMatrix4fv(glGetUniformLocation(m_ProgramID, vName.c_str()), 1, GL_FALSE, &vMat[0][0]));
}

void Elaina::CShaderProgram::__setUniform(const std::string& vName, const glm::mat3& vMat) const
{
	GL_SAFE_CALL(glUniformMatrix3fv(glGetUniformLocation(m_ProgramID, vName.c_str()), 1, GL_FALSE, &vMat[0][0]));
}

bool Elaina::CShaderProgram::setUniform(const std::string& vName, std::any vValue)
{
	if (m_Uniforms.count(vName) == 0)
	{
		spdlog::warn("uniform name: {} maybe not exist", vName);
		return false;
	}
	const auto& [Type, OldValue] = m_Uniforms[vName];
	if (!CGlslParser::checkValid(Type, vValue))
	{
		spdlog::error("uniform type and value not match {}", vName);
		return false;
	}
	m_Uniforms[vName] = { Type, vValue };
	return true;
}

bool Elaina::CShaderProgram::__dumpShaderCodeFromFile(const std::string& vShaderPath, EShaderType vShaderType, std::string& voShaderCode)
{
	std::ifstream ShaderFile;
	ShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		ShaderFile.open(vShaderPath);
		std::stringstream ShaderStream;
		ShaderStream << ShaderFile.rdbuf();
		ShaderFile.close();
		voShaderCode = ShaderStream.str();
		return true;
	}
	catch (std::ifstream::failure& Error)
	{
		spdlog::error("{} shader file READ failed: {}\n{}", __getShaderTypeName(vShaderType), vShaderPath, Error.what());
		return false;
	}
}

bool Elaina::CShaderProgram::__compileShader(const std::string& vShaderCode, EShaderType vShaderType, GLuint& voShaderID)
{
	voShaderID = glCreateShader(static_cast<GLenum>(vShaderType));
	const char* ShaderCodeCStr = vShaderCode.c_str();
	GL_SAFE_CALL(glShaderSource(voShaderID, 1, &ShaderCodeCStr, nullptr));
	GL_SAFE_CALL(glCompileShader(voShaderID));
	if (!__checkCompileError(voShaderID, vShaderType))
	{
		GL_SAFE_CALL(glDeleteShader(voShaderID));
		return false;
	}
	return true;
}

bool Elaina::CShaderProgram::__checkCompileError(GLuint vID, EShaderType vShaderType)
{
	GLint IsSuccess;
	GLchar InfoLog[1024];
	GL_SAFE_CALL(glGetShaderiv(vID, GL_COMPILE_STATUS, &IsSuccess));
	if (!IsSuccess)
	{
		GL_SAFE_CALL(glGetShaderInfoLog(vID, 1024, nullptr, InfoLog));
		spdlog::error("shader COMPILE failed: {}\n{}", __getShaderTypeName(vShaderType), InfoLog);
		return false;
	}
	return true;
}

bool Elaina::CShaderProgram::__checkLinkError(GLuint vID)
{
	GLint IsSuccess;
	GLchar InfoLog[1024];
	GL_SAFE_CALL(glGetProgramiv(vID, GL_LINK_STATUS, &IsSuccess));
	if (!IsSuccess)
	{
		GL_SAFE_CALL(glGetProgramInfoLog(vID, 1024, nullptr, InfoLog));
		spdlog::error("shader LINK failed: \n{}", InfoLog);
		return false;
	}
	return true;
}

std::string Elaina::CShaderProgram::__getShaderTypeName(EShaderType vShaderType)
{
	if (vShaderType == EShaderType::VERTEX) return "VERTEX";
	if (vShaderType == EShaderType::FRAGMENT) return "FRAGMENT";
	if (vShaderType == EShaderType::GEOMETRY) return "GEOMETRY";
	return "UNDEFINED";
}

void Elaina::CShaderProgram::__deleteShaderIDs()
{
	for (const auto& ShaderID : m_ShaderIDs)
	{
		GL_SAFE_CALL(glDeleteShader(ShaderID));
	}
	m_ShaderIDs.clear();
}
