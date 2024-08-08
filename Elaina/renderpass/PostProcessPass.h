#pragma once

#include "core/RenderPass.h"

namespace Elaina
{
	class CShaderProgram;
	class CVertexArrayObject;
	class CFrameBuffer;
	class CPostProcessPass final : public CRenderPass
	{
	public:
		explicit CPostProcessPass(const std::shared_ptr<CShaderProgram>& vShaderProgram, bool vIsFinalPass);

		void initV(int vWidth, int vHeight) override;
		void onWindowSizeChangeV(int vWidth, int vHeight) override;
		void renderV(const std::shared_ptr<CScene>& vScene) override;
		void setLastFrameBuffer(const std::shared_ptr<CFrameBuffer>& vLastFrameBuffer) { m_pLastFrameBuffer = vLastFrameBuffer; }
		[[nodiscard]] bool validateV() const override;

	private:
		std::shared_ptr<CVertexArrayObject> m_pQuadVAO;
		std::shared_ptr<CShaderProgram> m_pShaderProgram;
		std::shared_ptr<CFrameBuffer> m_pFrameBuffer;
		std::shared_ptr<CFrameBuffer> m_pLastFrameBuffer;
		bool m_IsFinalPass;
	};
}
