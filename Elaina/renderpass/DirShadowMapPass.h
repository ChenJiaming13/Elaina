#pragma once

#include "core/RenderPass.h"

namespace Elaina
{
	class CShaderProgram;
	struct SDirectionalLight;
	class CDirShadowMapPass : public CRenderPass
	{
	public:
		CDirShadowMapPass(const std::shared_ptr<CShaderProgram>& vShaderProgram);
		~CDirShadowMapPass();

		// Inherited via CRenderPass
		void renderV(const std::shared_ptr<CScene>& vScene, const std::vector<std::shared_ptr<CFrameBuffer>>& vFrameBuffers, const std::vector<size_t> vOutputIndices, size_t vIdxOfPasses) override;

	private:
		glm::mat4 __calcLightMatrix(const std::shared_ptr<SDirectionalLight>& vLight) const;

		std::shared_ptr<CShaderProgram> m_pShaderProgram;
		float m_Width, m_Height;
		float m_Near, m_Far;
	};
}