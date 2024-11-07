#pragma once

#include "base/Framebuffer.h"
#include "core/RenderPass.h"

namespace Elaina
{
	class CShaderProgram;
	class CFrameBuffer;
	class CDeferredGeoPass final : public IRenderPass
	{
	public:
		CDeferredGeoPass();

		void initV(int vWidth, int vHeight) override;
		void renderV(const std::shared_ptr<CScene>& vScene) override;
		void onWindowSizeChangeV(int vWidth, int vHeight) override;
		[[nodiscard]] const auto& getFrameBuffer() const { return m_pFrameBuffer; }

	private:
		std::shared_ptr<CShaderProgram> m_pShaderProgram;
		std::shared_ptr<CFrameBuffer> m_pFrameBuffer;
	};
}