#pragma once

#include <vector>
#include <memory>

namespace Elaina
{
	class CRenderPass;
	class CFrameBuffer;
	class CScene;
	class CRenderPipeline
	{
	public:
		CRenderPipeline() = default;

		void render(const std::shared_ptr<CScene>& vScene) const;
		void addRenderPass(const std::shared_ptr<CRenderPass>& vRenderPass, size_t vOutputIndex);
		void addFrameBuffer(const std::shared_ptr<CFrameBuffer>& vFrameBuffer) { m_FrameBuffers.push_back(vFrameBuffer); }
		void resize(int vWidth, int vHeight) const;

	private:
		std::vector<std::shared_ptr<CFrameBuffer>> m_FrameBuffers;
		std::vector<std::shared_ptr<CRenderPass>> m_RenderPasses;
		std::vector<size_t> m_OutputIndices;
	};
}