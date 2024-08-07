#pragma once

#include "base/Framebuffer.h"
#include "core/RenderPass.h"

namespace Elaina
{
	struct SPointLight;
	class CShaderProgram;
	class CFrameBuffer;
	class CPointShadowMapPass final : public CRenderPass
	{
	public:
		CPointShadowMapPass();

		void initV(int vWidth, int vHeight) override;
		void renderV(const std::shared_ptr<CScene>& vScene) override;
		void getShadowMapSize(int& voWidth, int& voHeight) const;
		[[nodiscard]] const auto& getShadowMap() const
		{
			return m_pFrameBuffer->getAttachment(GL_DEPTH_ATTACHMENT);
		}
		void setShadowMapSize(int vWidth, int vHeight) const;

	private:
		void __updateShadowTransforms(const std::shared_ptr<SPointLight>& vPointLight);

		std::shared_ptr<CFrameBuffer> m_pFrameBuffer;
		std::shared_ptr<CShaderProgram> m_pShaderProgram;
		std::vector<glm::mat4> m_LightMatrices;
		float m_Aspect = 1.0f;
	};
}
