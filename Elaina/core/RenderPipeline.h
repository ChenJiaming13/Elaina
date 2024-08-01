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
		void addRenderPass(const std::shared_ptr<CRenderPass>& vRenderPass, size_t vOutputIndex, bool vEnableAutoResize = true);
		void addFrameBuffer(const std::shared_ptr<CFrameBuffer>& vFrameBuffer) { m_FrameBuffers.push_back(vFrameBuffer); }
		void resize(int vWidth, int vHeight) const;
		void setPassEnable(size_t vIdxOfPasses, bool vEnable) { m_EnablePasses[vIdxOfPasses] = vEnable; }
		bool getPassEnable(size_t vIdxOfPasses) const { return m_EnablePasses[vIdxOfPasses]; }

	private:
		std::vector<std::shared_ptr<CFrameBuffer>> m_FrameBuffers;
		std::vector<std::shared_ptr<CRenderPass>> m_RenderPasses;
		std::vector<size_t> m_OutputIndices;
		std::vector<bool> m_EnablePasses;
		std::vector<bool> m_EnableAutoResize;
	};
}