#pragma once

#include "core/RenderPass.h"

namespace Elaina
{
	class CNode;
	class CVertexArrayObject;
	class CFrameBuffer;
	class CShaderProgram;
	class CDeferVisLightPass final : public CRenderPass
	{
	public:
		CDeferVisLightPass();
		~CDeferVisLightPass() override;

		void renderV(const std::shared_ptr<CScene>& vScene) override;
		void setLitFrameBuffer(const std::shared_ptr<CFrameBuffer>& vFrameBuffer) { m_pLitFrameBuffer = vFrameBuffer; }

	private:
		static glm::mat4 __calcRotationMatrix(const glm::vec3& vCurrDir, const glm::vec3& vTargetDir);

		std::shared_ptr<CFrameBuffer> m_pLitFrameBuffer;
		std::shared_ptr<CShaderProgram> m_pShaderProgram;
		std::shared_ptr<CVertexArrayObject> m_pPointLightVAO;
		std::shared_ptr<CNode> m_pArrowNode;
	};
}
