#include "pch.h"
#include "ShaderProgramManager.h"

using namespace Elaina::gl;

ProgramHandle CShaderProgramManager::createProgram(const SGraphicsProgramCreateInfo& vCreateInfo)
{
	std::set<ShaderHandle> Handles;
	if (!vCreateInfo._VertexShaderPath.empty())
	{
		if (const ShaderHandle Handle = __getOrCreateShader(EShaderType::VERTEX, vCreateInfo._VertexShaderPath); Handle != INVALID_SHADER_HANDLE)
			Handles.insert(Handle);
		else return INVALID_PROGRAM_HANDLE;
	}
	if (!vCreateInfo._FragmentShaderPath.empty())
	{
		if (const ShaderHandle Handle = __getOrCreateShader(EShaderType::FRAGMENT, vCreateInfo._FragmentShaderPath); Handle != INVALID_SHADER_HANDLE)
			Handles.insert(Handle);
		else return INVALID_PROGRAM_HANDLE;
	}
	if (!vCreateInfo._GeometryShaderPath.empty())
	{
		if (const ShaderHandle Handle = __getOrCreateShader(EShaderType::GEOMETRY, vCreateInfo._GeometryShaderPath); Handle != INVALID_SHADER_HANDLE)
			Handles.insert(Handle);
		else return INVALID_PROGRAM_HANDLE;
	}
	if (!vCreateInfo._TessControlShaderPath.empty())
	{
		if (const ShaderHandle Handle = __getOrCreateShader(EShaderType::TESS_CONTROL, vCreateInfo._TessControlShaderPath); Handle != INVALID_SHADER_HANDLE)
			Handles.insert(Handle);
		else return INVALID_PROGRAM_HANDLE;
	}
	if (!vCreateInfo._TessEvaluationShaderPath.empty())
	{
		if (const ShaderHandle Handle = __getOrCreateShader(EShaderType::TESS_EVALUATION, vCreateInfo._TessEvaluationShaderPath); Handle != INVALID_SHADER_HANDLE)
			Handles.insert(Handle);
		else return INVALID_PROGRAM_HANDLE;
	}
	return __createProgram(Handles);
}

ProgramHandle CShaderProgramManager::createProgram(const SComputeProgramCreateInfo& vCreateInfo)
{
	ShaderHandle Handle = __getOrCreateShader(EShaderType::COMPUTE, vCreateInfo._ComputeShaderPath);
	if (Handle == INVALID_SHADER_HANDLE) return INVALID_PROGRAM_HANDLE;
	return __createProgram({ Handle });
}

void CShaderProgramManager::useProgram(ProgramHandle vHandle) const
{
	assert(vHandle != INVALID_PROGRAM_HANDLE);
	assert(m_ProgramHandles.contains(vHandle));
	glUseProgram(vHandle);
}

void CShaderProgramManager::cleanup()
{
	for (const auto Handle : m_ProgramHandles | std::views::keys)
		glDeleteProgram(Handle);
	m_ProgramHandles.clear();
	for (const ShaderHandle Handle : m_ShaderHandles)
		glDeleteShader(Handle);
	m_ShaderHandles.clear();
	m_ShaderPathToHandle.clear();
}

void CShaderProgramManager::setUniform(ProgramHandle vHandle, const std::string& vName, int vValue)
{
	glUniform1iv(__queryUniformLocation(vHandle, vName), 1, &vValue);
}

void CShaderProgramManager::setUniform(ProgramHandle vHandle, const std::string& vName, float vValue)
{
	glUniform1fv(__queryUniformLocation(vHandle, vName), 1, &vValue);
}

void CShaderProgramManager::setUniform(ProgramHandle vHandle, const std::string& vName, const glm::vec2& vValue)
{
	glUniform2fv(__queryUniformLocation(vHandle, vName), 1, &vValue[0]);
}

void CShaderProgramManager::setUniform(ProgramHandle vHandle, const std::string& vName, const glm::vec3& vValue)
{
	glUniform3fv(__queryUniformLocation(vHandle, vName), 1, &vValue[0]);
}

void CShaderProgramManager::setUniform(ProgramHandle vHandle, const std::string& vName, const glm::vec4& vValue)
{
	glUniform4fv(__queryUniformLocation(vHandle, vName), 1, &vValue[0]);
}

void CShaderProgramManager::setUniform(ProgramHandle vHandle, const std::string& vName, const glm::mat4& vMat)
{
	glUniformMatrix4fv(__queryUniformLocation(vHandle, vName), 1, GL_FALSE, &vMat[0][0]);
}

void CShaderProgramManager::setUniform(ProgramHandle vHandle, const std::string& vName, const glm::mat3& vMat)
{
	glUniformMatrix3fv(__queryUniformLocation(vHandle, vName), 1, GL_FALSE, &vMat[0][0]);
}

ShaderHandle CShaderProgramManager::__getOrCreateShader(EShaderType vShaderType, const std::string& vShaderPath)
{
	if (m_ShaderPathToHandle.contains(vShaderPath)) return m_ShaderPathToHandle.at(vShaderPath);
	std::string ShaderCode;
	if (!__dumpShaderCodeFromFile(vShaderPath, ShaderCode)) return INVALID_SHADER_HANDLE;
	const auto Handle = __compileShader(vShaderType, ShaderCode);
	if (Handle != INVALID_SHADER_HANDLE)
	{
		m_ShaderPathToHandle[vShaderPath] = Handle;
		m_ShaderHandles.insert(Handle);
	}
	return Handle;
}

ProgramHandle CShaderProgramManager::__createProgram(const std::set<ShaderHandle>& vShaderHandles)
{
	const ProgramHandle Handle = glCreateProgram();
	for (const ShaderHandle ShaderHandle : vShaderHandles)
	{
		assert(m_ShaderHandles.contains(ShaderHandle));
		glAttachShader(Handle, ShaderHandle);
	}
	glLinkProgram(Handle);
	if (!__checkLinkError(Handle))
	{
		glDeleteProgram(Handle);
		return INVALID_PROGRAM_HANDLE;
	}
	m_ProgramHandles[Handle] = {};
	return Handle;
}

GLint CShaderProgramManager::__queryUniformLocation(ProgramHandle vHandle, const std::string& vUniformName)
{
	constexpr auto InvalidUniformLocation = -1;
	if (!m_ProgramHandles.contains(vHandle)) return InvalidUniformLocation;
	if (m_ProgramHandles.at(vHandle).contains(vUniformName))
		return m_ProgramHandles.at(vHandle).at(vUniformName);
	const auto UniformLocation = glGetUniformLocation(vHandle, vUniformName.c_str());
	if (UniformLocation == InvalidUniformLocation)
	{
		spdlog::error("uniform <{}> does not exist or is not set correctly", vUniformName);
		return InvalidUniformLocation;
	}
	m_ProgramHandles[vHandle][vUniformName] = UniformLocation;
	return UniformLocation;
}

bool CShaderProgramManager::__dumpShaderCodeFromFile(const std::string& vShaderPath, std::string& voShaderCode)
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
		spdlog::error("shader file <{}> READ failed: {}", vShaderPath, Error.what());
		return false;
	}
}

ShaderHandle CShaderProgramManager::__compileShader(EShaderType vShaderType, const std::string& vShaderCode)
{
	const ShaderHandle Handle = glCreateShader(static_cast<GLenum>(vShaderType));
	const char* ShaderCodeCStr = vShaderCode.c_str();
	glShaderSource(Handle, 1, &ShaderCodeCStr, nullptr);
	glCompileShader(Handle);
	if (!__checkCompileError(Handle, vShaderType))
	{
		glDeleteShader(Handle);
		return INVALID_SHADER_HANDLE;
	}
	return Handle;
}

bool CShaderProgramManager::__checkCompileError(ShaderHandle vHandle, EShaderType vShaderType)
{
	GLint IsSuccess;
	glGetShaderiv(vHandle, GL_COMPILE_STATUS, &IsSuccess);
	if (!IsSuccess)
	{
		GLchar InfoLog[1024];
		glGetShaderInfoLog(vHandle, 1024, nullptr, InfoLog);
		spdlog::error("shader <{}> COMPILE failed: {}", __getShaderTypeName(vShaderType), InfoLog);
		return false;
	}
	return true;
}

bool CShaderProgramManager::__checkLinkError(ProgramHandle vHandle)
{
	GLint IsSuccess;
	glGetProgramiv(vHandle, GL_LINK_STATUS, &IsSuccess);
	if (!IsSuccess)
	{
		GLchar InfoLog[1024];
		glGetProgramInfoLog(vHandle, 1024, nullptr, InfoLog);
		spdlog::error("program LINK failed: {}", InfoLog);
		return false;
	}
	return true;
}

std::string CShaderProgramManager::__getShaderTypeName(EShaderType vShaderType)
{
	if (vShaderType == EShaderType::VERTEX) return "VERTEX";
	if (vShaderType == EShaderType::FRAGMENT) return "FRAGMENT";
	if (vShaderType == EShaderType::GEOMETRY) return "GEOMETRY";
	if (vShaderType == EShaderType::COMPUTE) return "COMPUTE";
	if (vShaderType == EShaderType::TESS_CONTROL) return "TESS_CONTROL";
	if (vShaderType == EShaderType::TESS_EVALUATION) return "TESS_EVALUATION";
	return "UNDEFINED";
}
