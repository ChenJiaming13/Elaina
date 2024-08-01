#pragma once

#include "core/RenderPass.h"

namespace Elaina
{
	class CShaderProgram;
	struct SDirectionalLight;
	class CDirShadowMapPass : public CRenderPass
	{
	public:
		using CRenderPass::CRenderPass;

		// Inherited via CRenderPass
		void renderV(const std::shared_ptr<CScene>& vScene, const std::vector<std::shared_ptr<CFrameBuffer>>& vFrameBuffers, const std::vector<size_t> vOutputIndices, size_t vIdxOfPasses) override;

	private:
		glm::mat4 __calcLightMatrix(const std::shared_ptr<SDirectionalLight>& vLight) const;

		float m_Width = 20.0f;
		float m_Height = 20.0f;
		float m_Near = 0.1f;
		float m_Far = 20.0f;
	};
}