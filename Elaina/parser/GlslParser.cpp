#include "pch.h"
#include "GlslParser.h"
#include <regex>

Elaina::EUniformType Elaina::CGlslParser::queryUniformTypeByString(const std::string& vTypeStr)
{
	static const std::unordered_map<std::string, EUniformType> TypeMap = {
		{"float", EUniformType::FLOAT},
		{"vec2", EUniformType::VEC2},
		{"vec3", EUniformType::VEC3},
		{"vec4", EUniformType::VEC4},
		{"mat3", EUniformType::MAT3},
		{"mat4", EUniformType::MAT4},
		{"sampler2D", EUniformType::SAMPLER2D},
	};

	auto it = TypeMap.find(vTypeStr);
	if (it != TypeMap.end()) return it->second;
	return EUniformType::UNKNOWN;
}

std::any Elaina::CGlslParser::getDefaultValue(EUniformType vType)
{
	switch (vType)
	{
	case EUniformType::FLOAT:
		return 0.0f;
	case EUniformType::VEC2:
		return glm::vec2(0.0f, 0.0f);
	case EUniformType::VEC3:
		return glm::vec3(0.0f, 0.0f, 0.0f);
	case EUniformType::VEC4:
		return glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
	case EUniformType::MAT3:
		return glm::mat3(1.0f);
	case EUniformType::MAT4:
		return glm::mat4(1.0f);
	case EUniformType::SAMPLER2D:
		return 0;
	default:
		return std::any();
	}
}

bool Elaina::CGlslParser::checkValid(EUniformType vType, std::any vValue)
{
	try
	{
		switch (vType)
		{
		case EUniformType::FLOAT:
			return vValue.type() == typeid(float);
		case EUniformType::VEC2:
			return vValue.type() == typeid(glm::vec2);
		case EUniformType::VEC3:
			return vValue.type() == typeid(glm::vec3);
		case EUniformType::VEC4:
			return vValue.type() == typeid(glm::vec4);
		case EUniformType::MAT3:
			return vValue.type() == typeid(glm::mat3);
		case EUniformType::MAT4:
			return vValue.type() == typeid(glm::mat4);
		case EUniformType::SAMPLER2D:
			return vValue.type() == typeid(int);
		default:
			return false; // unknown or match error
		}
	}
	catch (const std::bad_any_cast e)
	{
		return false;
	}
}

void Elaina::CGlslParser::parseUniforms(const std::string& vShaderCode, std::unordered_map<std::string, EUniformType>& voUniforms)
{
	//voUniforms.clear();

	// delete comments (single & multi line)
	std::string ShaderCode = vShaderCode;
	std::regex SingleLineComment(R"(//.*)");
	ShaderCode = std::regex_replace(ShaderCode, SingleLineComment, "");
	std::regex MultiLineComment(R"(/\*[\s\S]*?\*/)");
	ShaderCode = std::regex_replace(ShaderCode, MultiLineComment, "");

	std::regex UniformRegex(R"(uniform\s+(\w+)\s+(\w+)\s*;)");
	std::smatch Match;

	std::string::const_iterator SearchStart(ShaderCode.cbegin());
	while (std::regex_search(SearchStart, ShaderCode.cend(), Match, UniformRegex))
	{
		if (Match.size() == 3)
		{
			// match[0]是整个匹配，match[1]是类型，match[2]是名称
			std::string TypeStr = Match[1];
			std::string Name = Match[2];
			voUniforms[Name] = queryUniformTypeByString(TypeStr);
		}
		SearchStart = Match.suffix().first;
	}
}
