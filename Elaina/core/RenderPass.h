#pragma once

#include <memory>
#include <vector>

namespace Elaina
{
	class CScene;
	class CFrameBuffer;
	class CRenderPass
	{
	public:
		virtual ~CRenderPass() = default;

		virtual void renderV(
			const std::shared_ptr<CScene>& vScene, 
			const std::vector<std::shared_ptr<CFrameBuffer>>& vFrameBuffers, 
			const std::vector<size_t> vOutputIndices,
			size_t vIdxOfPasses
		);
	};
}