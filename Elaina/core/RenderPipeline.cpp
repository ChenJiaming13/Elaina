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
			if (m_FuncPrevRenders[i]) m_FuncPrevRenders[i](vScene);
			m_RenderPasses[i]->renderV(vScene);
			if (m_FuncPostRenders[i]) m_FuncPostRenders[i](vScene);
		}
	}
}

void Elaina::CRenderPipeline::addRenderPass(
	const std::shared_ptr<IRenderPass>& vRenderPass,
	const std::function<void(const std::shared_ptr<CScene>& vScene)>& vFuncPrevRender,
	const std::function<void(const std::shared_ptr<CScene>& vScene)>& vFuncPostRender)
{
	m_RenderPasses.push_back(vRenderPass);
	m_FuncPrevRenders.push_back(vFuncPrevRender);
	m_FuncPostRenders.push_back(vFuncPostRender);
	m_EnablePasses.push_back(true);
}

void Elaina::CRenderPipeline::validate() const
{
	for (size_t i = 0; i < m_RenderPasses.size(); ++i)
	{
		if (!m_RenderPasses[i]->validateV())
		{
			spdlog::error("idx of {} render passes validate failed", i);
			throw std::runtime_error("validate failed");
		}
	}
	spdlog::info("render pipeline validate success");
}