#include "pch.h"
#include "Material.h"
#include "base/ShaderProgram.h"
#include "parser/GlslParser.h"

Elaina::CMaterial::CMaterial(const std::shared_ptr<CShaderProgram>& vShaderProgram)
	:m_pShaderProgram(vShaderProgram)
{
}

Elaina::CMaterial::~CMaterial()
{
	m_pShaderProgram.reset();
}

bool Elaina::CMaterial::checkUniformExist(const std::string& vName) const
{
	return m_pShaderProgram->getUniforms().count(vName) != 0;
}

bool Elaina::CMaterial::setUniform(const std::string& vName, std::any vValue)
{
	const auto& Uniforms = m_pShaderProgram->getUniforms();
	const auto& it = Uniforms.find(vName);
	if (it == Uniforms.end())
	{
		spdlog::warn("uniform name: {} maybe not exist", vName);
		return false;
	}
	if (!CGlslParser::checkValid(it->second, vValue))
	{
		spdlog::error("uniform type and value not match {}", vName);
		return false;
	}
	__setUniform(vName, it->second, vValue);
	return true;
}

void Elaina::CMaterial::use() const
{
	m_pShaderProgram->use();
}

void Elaina::CMaterial::__setUniform(const std::string& vName, EUniformType vType, std::any vValue) const
{
	switch (vType)
	{
	case EUniformType::FLOAT:
		m_pShaderProgram->setUniform(vName, std::any_cast<float>(vValue));
		break;
	case EUniformType::VEC2:
		m_pShaderProgram->setUniform(vName, std::any_cast<glm::vec2>(vValue));
		break;
	case EUniformType::VEC3:
		m_pShaderProgram->setUniform(vName, std::any_cast<glm::vec3>(vValue));
		break;
	case EUniformType::VEC4:
		m_pShaderProgram->setUniform(vName, std::any_cast<glm::vec4>(vValue));
		break;
	case EUniformType::MAT3:
		m_pShaderProgram->setUniform(vName, std::any_cast<glm::mat3>(vValue));
		break;
	case EUniformType::MAT4:
		m_pShaderProgram->setUniform(vName, std::any_cast<glm::mat4>(vValue));
		break;
	case EUniformType::SAMPLER2D:
		m_pShaderProgram->setUniform(vName, std::any_cast<int>(vValue));
		break;
	default:
		spdlog::error("unsupported uniform type for: {}", vName);
	}
}
