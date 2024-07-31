#pragma once

#include "core/RenderPass.h"

namespace Elaina
{
	class CShaderProgram;
	class CVertexArrayObject;
	class CPostProcessPass : public CRenderPass
	{
	public:
		CPostProcessPass(const std::shared_ptr<CShaderProgram>& vShaderProgram);
		~CPostProcessPass();

		// Inherited via CRenderPass
		void renderV(const std::shared_ptr<CScene>& vScene, const std::vector<std::shared_ptr<CFrameBuffer>>& vFrameBuffers, const std::vector<size_t> vOutputIndices, size_t vIdxOfPasses) override;

	private:
		std::shared_ptr<CShaderProgram> m_pShaderProgram;
		std::shared_ptr<CVertexArrayObject> m_pQuadVAO;
	};
}