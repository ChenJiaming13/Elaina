#pragma once

#include "core/RenderPass.h"

namespace Elaina
{
	class CShaderProgram;
	class CFrameBuffer;
	class CForwardLitPass final : public IRenderPass
	{
	public:
		explicit CForwardLitPass(bool vIsFinalPass);
		~CForwardLitPass() override;

		void initV(int vWidth, int vHeight) override;
		void onWindowSizeChangeV(int vWidth, int vHeight) override;
		void renderV(const std::shared_ptr<CScene>& vScene) override;
		[[nodiscard]] const auto& getFrameBuffer() const { return m_pFrameBuffer; }
		void setEnableClipPlane(bool vEnableClipPlane) { m_EnableClipPlane = vEnableClipPlane; }
		void setClipPlane(const glm::vec4& vClipPlane) { m_ClipPlane = vClipPlane; }

	private:
		std::shared_ptr<CFrameBuffer> m_pFrameBuffer;
		std::shared_ptr<CShaderProgram> m_pShaderProgramPBR;
		std::shared_ptr<CShaderProgram> m_pShaderProgramPhong;
		std::shared_ptr<CShaderProgram> m_pShaderProgramChecker;
		bool m_IsFinalPass;
		bool m_EnableClipPlane = false;
		glm::vec4 m_ClipPlane;
	};
}