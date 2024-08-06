#pragma once

#include "core/RenderPass.h"

namespace Elaina
{
	class CVertexArrayObject;
	class CDirShadowMapPass;
	class CDeferredLitPass final : public CRenderPass
	{
	public:
		CDeferredLitPass(
			size_t vIdxOfDeferredGeoFB,
			size_t vIdxOfDirShadowMapFB,
			size_t vIdxOfPointShadowMapFB,
			const std::shared_ptr<CDirShadowMapPass>& vDirShadowMapPass
		);
		~CDeferredLitPass() override;

		// Inherited via CRenderPass
		void renderV(const std::shared_ptr<CScene>& vScene, const std::vector<std::shared_ptr<CFrameBuffer>>& vFrameBuffers, const std::vector<size_t> vOutputIndices, size_t vIdxOfPasses) override;
		void setEnablePCF(bool vEnablePCF) { m_EnablePCF = vEnablePCF; }
		void setHalfSizePCF(int vHalfSizePCF)
		{
			if (vHalfSizePCF < 0) return;
			m_HalfSizePCF = vHalfSizePCF;
		}
		[[nodiscard]] bool getEnablePCF() const { return m_EnablePCF; }
		[[nodiscard]] int getHalfSizePCF() const { return m_HalfSizePCF; }

	private:
		std::shared_ptr<CVertexArrayObject> m_pQuadVAO;
		std::shared_ptr<CDirShadowMapPass> m_pDirShadowMapPass;
		size_t m_IdxOfDeferredGeoFB;
		size_t m_IdxOfDirShadowMapFB;
		size_t m_IdxOfPointShadowMapFB;
		bool m_EnablePCF;
		int m_HalfSizePCF;
	};
}