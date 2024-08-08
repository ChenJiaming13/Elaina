#pragma once

#include "core/RenderPass.h"

namespace Elaina
{
	class CTextureCube;
	class CVertexArrayObject;
	class CFrameBuffer;
	class CShaderProgram;
	class CSkyBoxPass final : public CRenderPass
	{
	public:
		explicit CSkyBoxPass(const std::array<std::string, 6>& vCubeMapFiles, bool vIsDeferred);

		void renderV(const std::shared_ptr<CScene>& vScene) override;
		void setGeoFrameBuffer(const std::shared_ptr<CFrameBuffer>& vFrameBuffer) { m_pGeoFrameBuffer = vFrameBuffer; }
		void setLitFrameBuffer(const std::shared_ptr<CFrameBuffer>& vFrameBuffer) { m_pLitFrameBuffer = vFrameBuffer; }

	private:
		std::shared_ptr<CTextureCube> m_pCubeMap;
		std::shared_ptr<CVertexArrayObject>m_pSkyBoxVAO;
		std::shared_ptr<CFrameBuffer> m_pGeoFrameBuffer = nullptr;
		std::shared_ptr<CFrameBuffer> m_pLitFrameBuffer = nullptr;
		std::shared_ptr<CShaderProgram> m_pShaderProgram;
		bool m_IsDeferred;
	};
}
