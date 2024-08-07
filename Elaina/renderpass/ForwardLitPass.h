#pragma once

#include "core/RenderPass.h"

namespace Elaina
{
	class CForwardLitPass final : public CRenderPass
	{
	public:
		CForwardLitPass();
		~CForwardLitPass() override;

		// Inherited via CRenderPass
		void renderV(const std::shared_ptr<CScene>& vScene, const std::vector<std::shared_ptr<CFrameBuffer>>& vFrameBuffers, const std::vector<size_t>& vOutputIndices, size_t vIdxOfPasses) override;

	private:
		std::shared_ptr<CShaderProgram> m_pShaderProgramPBR;
		std::shared_ptr<CShaderProgram> m_pShaderProgramPhong;
		std::shared_ptr<CShaderProgram> m_pShaderProgramChecker;
	};
}