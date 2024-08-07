#pragma once

#include "core/RenderPass.h"

namespace Elaina
{
	class CShaderProgram;
	class CVertexArrayObject;
	class CPostProcessPass final : public CRenderPass
	{
	public:
		explicit CPostProcessPass(const std::shared_ptr<CShaderProgram>& vShaderProgram);
		~CPostProcessPass() override;

		// Inherited via CRenderPass
		void renderV(const std::shared_ptr<CScene>& vScene, const std::vector<std::shared_ptr<CFrameBuffer>>& vFrameBuffers, const std::vector<size_t>& vOutputIndices, size_t vIdxOfPasses) override;

	private:
		std::shared_ptr<CVertexArrayObject> m_pQuadVAO;
	};
}
