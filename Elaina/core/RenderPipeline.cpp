#include "pch.h"
#include "RenderPipeline.h"
#include "RenderPass.h"

void Elaina::CRenderPipeline::init(int vWidth, int vHeight) const
{
	for (const auto& pRenderPass : m_RenderPasses)
	{
		pRenderPass->initV(vWidth, vHeight);
	}
}

void Elaina::CRenderPipeline::onWindowSizeChange(int vWidth, int vHeight) const
{
	for (const auto& pRenderPass : m_RenderPasses)
	{
		pRenderPass->onWindowSizeChangeV(vWidth, vHeight);
	}
}

void Elaina::CRenderPipeline::render(const std::shared_ptr<CScene>& vScene) const
{
	for (size_t i = 0; i < m_RenderPasses.size(); ++i)
	{
		if (m_EnablePasses[i])
		{
			m_RenderPasses[i]->renderV(vScene);
		}
	}
}

void Elaina::CRenderPipeline::addRenderPass(const std::shared_ptr<CRenderPass>& vRenderPass)
{
	m_RenderPasses.push_back(vRenderPass);
	m_EnablePasses.push_back(true);
}
