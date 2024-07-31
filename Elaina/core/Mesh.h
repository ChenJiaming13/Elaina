#pragma once

#include <memory>
#include "base/VertexArrayObject.h"

namespace Elaina
{
	class CShaderProgram;
	class CMesh
	{
	public:
		CMesh(const std::shared_ptr<CVertexArrayObject>& vVAO, const std::shared_ptr<CShaderProgram>& vShaderProgram = nullptr)
			:m_pVAO(vVAO), m_pShaderProgram(vShaderProgram) {}

		void draw() const
		{
			m_pVAO->bind();
			m_pVAO->draw();
		}

		void setShaderProgram(const std::shared_ptr<CShaderProgram>& vShaderProgram) { m_pShaderProgram = vShaderProgram; }
		const auto& getShaderProgram() const { return m_pShaderProgram; }

	private:
		std::shared_ptr<CVertexArrayObject> m_pVAO;
		std::shared_ptr<CShaderProgram> m_pShaderProgram;
	};
}