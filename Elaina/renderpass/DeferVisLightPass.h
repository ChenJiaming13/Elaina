#pragma once

#include "core/RenderPass.h"

namespace Elaina
{
	class CNode;
	class CVertexArrayObject;

	class CDeferVisLightPass final : public CRenderPass
	{
	public:
		CDeferVisLightPass();
		~CDeferVisLightPass() override;

		void renderV(const std::shared_ptr<CScene>& vScene,
			const std::vector<std::shared_ptr<CFrameBuffer>>& vFrameBuffers, const std::vector<size_t>& vOutputIndices,
			size_t vIdxOfPasses) override;

	private:
		static glm::mat4 __calcRotationMatrix(const glm::vec3& vCurrDir, const glm::vec3& vTargetDir);

		std::shared_ptr<CVertexArrayObject> m_pPointLightVAO;
		std::shared_ptr<CNode> m_pArrowNode;
	};
}
