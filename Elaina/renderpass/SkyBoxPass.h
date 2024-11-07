#pragma once

#include "core/RenderPass.h"

namespace Elaina
{
	class CTextureCube;
	class CVertexArrayObject;
	class CFrameBuffer;
	class CShaderProgram;
	class CSkyBoxPass final : public IRenderPass
	{
	public:
		explicit CSkyBoxPass(const std::array<std::string, 6>& vCubeMapFiles);

		void renderV(const std::shared_ptr<CScene>& vScene) override;
		void setLitFrameBuffer(const std::shared_ptr<CFrameBuffer>& vFrameBuffer) { m_pLitFrameBuffer = vFrameBuffer; }
		[[nodiscard]] bool validateV() const override;
		
	private:
		std::shared_ptr<CTextureCube> m_pCubeMap;
		std::shared_ptr<CVertexArrayObject>m_pSkyBoxVAO;
		std::shared_ptr<CFrameBuffer> m_pLitFrameBuffer = nullptr;
		std::shared_ptr<CShaderProgram> m_pShaderProgram;
	};
}
