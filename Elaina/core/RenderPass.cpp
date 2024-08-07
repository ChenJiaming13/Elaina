#include "pch.h"
#include "RenderPass.h"
#include "base/Framebuffer.h"
#include "safe.h"

void Elaina::CRenderPass::renderV(const std::shared_ptr<CScene>& vScene, const std::vector<std::shared_ptr<CFrameBuffer>>& vFrameBuffers, const std::vector<size_t>& vOutputIndices, size_t vIdxOfPasses)
{
	const auto& pFrameBuffer = vFrameBuffers[vOutputIndices[vIdxOfPasses]];
	pFrameBuffer->bind();
	GL_SAFE_CALL(glViewport(0, 0, pFrameBuffer->getWidth(), pFrameBuffer->getHeight()));
}
