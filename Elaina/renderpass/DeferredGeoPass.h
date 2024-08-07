#pragma once

#include "core/RenderPass.h"

namespace Elaina
{
	class CShaderProgram;
	class CDeferredGeoPass final : public CRenderPass
	{
	public:
		CDeferredGeoPass();

		// Inherited via CRenderPass
		void renderV(const std::shared_ptr<CScene>& vScene, const std::vector<std::shared_ptr<CFrameBuffer>>& vFrameBuffers, const std::vector<size_t>& vOutputIndices, size_t vIdxOfPasses) override;
	};
}