#pragma once

#include <set>
#include <map>
#include <string>
#include <glm/glm.hpp>

namespace Elaina::gl
{
	using ShaderHandle = GLuint;
	using ProgramHandle = GLuint;

	constexpr auto INVALID_SHADER_HANDLE = std::numeric_limits<ShaderHandle>::max();
	constexpr auto INVALID_PROGRAM_HANDLE = std::numeric_limits<ProgramHandle>::max();

	enum class EShaderType
	{
		VERTEX			= GL_VERTEX_SHADER,
		TESS_CONTROL	= GL_TESS_CONTROL_SHADER,
		TESS_EVALUATION	= GL_TESS_EVALUATION_SHADER,
		GEOMETRY		= GL_GEOMETRY_SHADER,
		FRAGMENT		= GL_FRAGMENT_SHADER,
		COMPUTE			= GL_COMPUTE_SHADER,
	};

	struct SGraphicsProgramCreateInfo
	{
		std::string _VertexShaderPath;
		std::string _TessControlShaderPath;
		std::string _TessEvaluationShaderPath;
		std::string _GeometryShaderPath;
		std::string _FragmentShaderPath;
	};

	struct SComputeProgramCreateInfo
	{
		std::string _ComputeShaderPath;
	};

	class CShaderProgramManager
	{
	public:
		~CShaderProgramManager() { cleanup(); }

		ProgramHandle createProgram(const SGraphicsProgramCreateInfo& vCreateInfo);
		ProgramHandle createProgram(const SComputeProgramCreateInfo& vCreateInfo);
		void useProgram(ProgramHandle vHandle) const;
		void cleanup();

		void setUniform(ProgramHandle vHandle, const std::string& vName, int vValue);
		void setUniform(ProgramHandle vHandle, const std::string& vName, float vValue);
		void setUniform(ProgramHandle vHandle, const std::string& vName, const glm::vec2& vValue);
		void setUniform(ProgramHandle vHandle, const std::string& vName, const glm::vec3& vValue);
		void setUniform(ProgramHandle vHandle, const std::string& vName, const glm::vec4& vValue);
		void setUniform(ProgramHandle vHandle, const std::string& vName, const glm::mat4& vMat);
		void setUniform(ProgramHandle vHandle, const std::string& vName, const glm::mat3& vMat);

	private:
		ShaderHandle __getOrCreateShader(EShaderType vShaderType, const std::string& vShaderPath);
		ProgramHandle __createProgram(const std::set<ShaderHandle>& vShaderHandles);
		GLint __queryUniformLocation(ProgramHandle vHandle, const std::string& vUniformName);

		static bool __dumpShaderCodeFromFile(const std::string& vShaderPath, std::string& voShaderCode);
		static ShaderHandle __compileShader(EShaderType vShaderType, const std::string& vShaderCode);
		static bool __checkCompileError(ShaderHandle vHandle, EShaderType vShaderType);
		static bool __checkLinkError(ProgramHandle vHandle);
		static std::string __getShaderTypeName(EShaderType vShaderType);

		std::map<std::string, ShaderHandle> m_ShaderPathToHandle;
		std::set<ShaderHandle> m_ShaderHandles;
		std::map<ProgramHandle, std::map<std::string, GLint>> m_ProgramHandles;
	};
}
