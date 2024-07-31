#pragma once

#include <unordered_map>
#include <any>

namespace Elaina
{
	class CShaderProgram;
	enum class EUniformType;
	class CMaterial
	{
	public:
		CMaterial(const std::shared_ptr<CShaderProgram>& vShaderProgram);
		~CMaterial();

		bool checkUniformExist(const std::string& vName) const;
		bool setUniform(const std::string& vName, std::any vValue);
		void use() const;

	private:
		void __setUniform(const std::string& vName, EUniformType vType, std::any vValue) const;

		std::shared_ptr<CShaderProgram> m_pShaderProgram;
	};
}