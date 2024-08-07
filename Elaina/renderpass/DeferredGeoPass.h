#pragma once

#include "base/Framebuffer.h"
#include "core/RenderPass.h"

namespace Elaina
{
	class CShaderProgram;
	class CFrameBuffer;
	class CDeferredGeoPass final : public CRenderPass
	{
	public:
		CDeferredGeoPass();

		void initV(int vWidth, int vHeight) override;
		void renderV(const std::shared_ptr<CScene>& vScene) override;
		void onWindowSizeChangeV(int vWidth, int vHeight) override;
		[[nodiscard]] const auto& getFrameBuffer() const { return m_pFrameBuffer; }
		[[nodiscard]] const auto& getGeoPositionTex() const
		{
			return m_pFrameBuffer->getAttachment(GL_COLOR_ATTACHMENT0);
		}
		[[nodiscard]] const auto& getGeoNormalTex() const
		{
			return m_pFrameBuffer->getAttachment(GL_COLOR_ATTACHMENT1);
		}
		[[nodiscard]] const auto& getGeoDepthTex() const
		{
			return m_pFrameBuffer->getAttachment(GL_DEPTH_ATTACHMENT);
		}
		[[nodiscard]] const auto& getGeoAlbedoTex() const
		{
			return m_pFrameBuffer->getAttachment(GL_COLOR_ATTACHMENT2);
		}
		[[nodiscard]] const auto& getGeoPbrPropsTex() const
		{
			return m_pFrameBuffer->getAttachment(GL_COLOR_ATTACHMENT3);
		}

	private:
		std::shared_ptr<CShaderProgram> m_pShaderProgram;
		std::shared_ptr<CFrameBuffer> m_pFrameBuffer;
	};
}