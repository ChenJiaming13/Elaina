#pragma once

#include "core/RenderPass.h"

namespace Elaina
{
	class CVertexArrayObject;

	class CDeferVisLightPass final : public CRenderPass
	{
	public:
		explicit CDeferVisLightPass(const std::shared_ptr<CShaderProgram>& vShaderProgram);
		~CDeferVisLightPass() override;

		void renderV(const std::shared_ptr<CScene>& vScene,
			const std::vector<std::shared_ptr<CFrameBuffer>>& vFrameBuffers, const std::vector<size_t> vOutputIndices,
			size_t vIdxOfPasses) override;

	private:
		std::shared_ptr<CVertexArrayObject> m_pLightVAO;
	};
}
