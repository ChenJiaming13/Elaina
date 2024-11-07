#pragma once

#include "base/Framebuffer.h"
#include "core/RenderPass.h"

namespace Elaina
{
	class CShaderProgram;
	class CFrameBuffer;
	class CDirShadowMapPass final : public IRenderPass
	{
	public:
		CDirShadowMapPass();

		void initV(int vWidth, int vHeight) override;
		void renderV(const std::shared_ptr<CScene>& vScene) override;
		void getShadowMapSize(int& voWidth, int& voHeight) const;
		[[nodiscard]] const auto& getShadowMap() const
		{
			return m_pFrameBuffer->getAttachment(GL_DEPTH_ATTACHMENT);
		}
		void setShadowMapSize(int vWidth, int vHeight) const;

	private:
		std::shared_ptr<CShaderProgram> m_pShaderProgram;
		std::shared_ptr<CFrameBuffer> m_pFrameBuffer;
	};
}