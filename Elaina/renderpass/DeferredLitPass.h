#pragma once

#include "core/RenderPass.h"

namespace Elaina
{
	class CVertexArrayObject;
	class CDirShadowMapPass;
	class CDeferredLitPass : public CRenderPass
	{
	public:
		CDeferredLitPass(
			const std::shared_ptr<CShaderProgram>& vShaderProgram,
			size_t vIdxOfDeferredGeoFB,
			size_t vIdxOfDirShadowMapFB,
			size_t vIdxOfPointShadowMapFB,
			const std::shared_ptr<CDirShadowMapPass>& vDirShadowMapPass
		);
		~CDeferredLitPass();

		// Inherited via CRenderPass
		void renderV(const std::shared_ptr<CScene>& vScene, const std::vector<std::shared_ptr<CFrameBuffer>>& vFrameBuffers, const std::vector<size_t> vOutputIndices, size_t vIdxOfPasses) override;

	private:
		std::shared_ptr<CVertexArrayObject> m_pQuadVAO;
		std::shared_ptr<CDirShadowMapPass> m_pDirShadowMapPass;
		size_t m_IdxOfDeferredGeoFB;
		size_t m_IdxOfDirShadowMapFB;
		size_t m_IdxOfPointShadowMapFB;
	};
}