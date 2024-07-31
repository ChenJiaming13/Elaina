#pragma once

#include <unordered_map>
#include <string>
#include <any>

namespace Elaina
{
	enum class EUniformType {
		UNKNOWN,
		FLOAT,
		VEC2,
		VEC3,
		VEC4,
		MAT3,
		MAT4,
		SAMPLER2D,
	};

	class CGlslParser
	{
	public:
		static EUniformType queryUniformTypeByString(const std::string& vTypeStr);
		static std::any getDefaultValue(EUniformType vType);
		static bool checkValid(EUniformType vType, std::any vValue);
		static void parseUniforms(const std::string& vShaderCode, std::unordered_map<std::string, std::pair<EUniformType, std::any>>& voUniforms);
	};
}