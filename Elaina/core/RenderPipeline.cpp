#include "pch.h"
#include "RenderPipeline.h"
#include "RenderPass.h"
#include "base/Framebuffer.h"

void Elaina::CRenderPipeline::render(const std::shared_ptr<CScene>& vScene) const
{
	for (size_t i = 0; i < m_RenderPasses.size(); ++i)
	{
		m_RenderPasses[i]->renderV(vScene, m_FrameBuffers, m_OutputIndices, i);
	}
}

void Elaina::CRenderPipeline::addRenderPass(const std::shared_ptr<CRenderPass>& vRenderPass, size_t vOutputIndex)
{
	m_RenderPasses.push_back(vRenderPass);
	m_OutputIndices.push_back(vOutputIndex);
}

void Elaina::CRenderPipeline::resize(int vWidth, int vHeight) const
{
	for (const auto& pFrameBuffer : m_FrameBuffers)
	{
		pFrameBuffer->resize(vWidth, vHeight);
	}
}