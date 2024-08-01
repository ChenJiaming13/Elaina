#pragma once

#include "core/RenderPass.h"

namespace Elaina
{
	class CTextureCube;
	class CVertexArrayObject;
	class CDeferredSkyBoxPass : public CRenderPass
	{
	public:
		CDeferredSkyBoxPass(const std::shared_ptr<CShaderProgram>& vShaderProgram, const std::shared_ptr<CTextureCube>& vCubeMap, size_t vIdxOfDeferredGeoFB);
		~CDeferredSkyBoxPass();

		// Inherited via CRenderPass
		void renderV(const std::shared_ptr<CScene>& vScene, const std::vector<std::shared_ptr<CFrameBuffer>>& vFrameBuffers, const std::vector<size_t> vOutputIndices, size_t vIdxOfPasses) override;

	private:
		std::shared_ptr<CTextureCube> m_pCubeMap;
		std::shared_ptr<CVertexArrayObject>m_pSkyBoxVAO;
		size_t m_IdxOfDeferredGeoFB;
	};
}