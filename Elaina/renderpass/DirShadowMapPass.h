#pragma once

#include "core/RenderPass.h"

namespace Elaina
{
	class CShaderProgram;
	struct SDirectionalLight;
	class CDirShadowMapPass final : public CRenderPass
	{
	public:
		using CRenderPass::CRenderPass;

		// Inherited via CRenderPass
		void renderV(const std::shared_ptr<CScene>& vScene, const std::vector<std::shared_ptr<CFrameBuffer>>& vFrameBuffers, const std::vector<size_t> vOutputIndices, size_t vIdxOfPasses) override;
		[[nodiscard]] glm::mat4 calcLightMatrix(const std::shared_ptr<SDirectionalLight>& vLight) const;

	private:
		float m_Width = 10.0f;
		float m_Height = 10.0f;
		float m_Near = 0.1f;
		float m_Far = 20.0f;
	};
}