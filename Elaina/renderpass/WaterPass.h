#pragma once
#include "core/RenderPass.h"

namespace Elaina
{
	class CShaderProgram;
	class CFrameBuffer;
	class CWaterPass final : public CRenderPass
	{
	public:
		CWaterPass();

		void renderV(const std::shared_ptr<CScene>& vScene) override;
		void setLitFrameBuffer(const std::shared_ptr<CFrameBuffer>& vFrameBuffer) { m_pLitFrameBuffer = vFrameBuffer; }
		[[nodiscard]] bool validateV() const override;

	private:
		std::shared_ptr<CShaderProgram> m_pShaderProgram;
		std::shared_ptr<CFrameBuffer> m_pLitFrameBuffer;
	};
}
