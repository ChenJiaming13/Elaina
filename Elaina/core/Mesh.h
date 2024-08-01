#pragma once

#include <memory>
#include "base/VertexArrayObject.h"

namespace Elaina
{
	struct SMaterial;
	class CMesh
	{
	public:
		CMesh(const std::shared_ptr<CVertexArrayObject>& vVAO, const std::shared_ptr<SMaterial>& vMaterial = nullptr)
			:m_pVAO(vVAO), m_pMaterial(vMaterial) {}

		void draw() const
		{
			m_pVAO->bind();
			m_pVAO->draw();
		}

		void setMaterial(const std::shared_ptr<SMaterial>& vMaterial) { m_pMaterial = vMaterial; }
		const auto& getMaterial() const { return m_pMaterial; }

	private:
		std::shared_ptr<CVertexArrayObject> m_pVAO;
		std::shared_ptr<SMaterial> m_pMaterial;
	};
}