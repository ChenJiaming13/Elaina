#pragma once

#include <memory>
#include "base/VertexArrayObject.h"

namespace Elaina
{
	class CMaterial;
	class CMesh
	{
	public:
		CMesh(const std::shared_ptr<CVertexArrayObject>& vVAO, const std::shared_ptr<CMaterial>& vMaterial = nullptr)
			:m_pVAO(vVAO), m_pMaterial(vMaterial) {}

		void draw() const
		{
			m_pVAO->bind();
			m_pVAO->draw();
		}

		void setMaterial(const std::shared_ptr<CMaterial>& vMaterial) { m_pMaterial = vMaterial; }
		const auto& getMaterial() const { return m_pMaterial; }

	private:
		std::shared_ptr<CVertexArrayObject> m_pVAO;
		std::shared_ptr<CMaterial> m_pMaterial;
	};
}