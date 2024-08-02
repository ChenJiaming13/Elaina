#pragma once

#include "core/RenderPass.h"

namespace Elaina
{
	struct SPointLight;

	class CPointShadowMapPass final : public CRenderPass
	{
	public:
		using CRenderPass::CRenderPass;

		void renderV(const std::shared_ptr<CScene>& vScene,
			const std::vector<std::shared_ptr<CFrameBuffer>>& vFrameBuffers, const std::vector<size_t> vOutputIndices,
			size_t vIdxOfPasses) override;

	private:
		void __updateShadowTransforms(const std::shared_ptr<SPointLight>& vPointLight);

		std::vector<glm::mat4> m_LightMatrices;
		float m_Aspect = 1.0f;
	};
}
