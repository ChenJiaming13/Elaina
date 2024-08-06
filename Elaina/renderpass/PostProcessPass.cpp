#include "pch.h"
#include "PostProcessPass.h"
#include "base/Framebuffer.h"
#include "base/Texture2D.h"
#include "base/ShaderProgram.h"
#include "base/VertexArrayObject.h"
#include "primitive/Primitive.h"
#include "safe.h"

Elaina::CPostProcessPass::CPostProcessPass(const std::shared_ptr<CShaderProgram>& vShaderProgram)
	:CRenderPass(vShaderProgram), m_pQuadVAO(CPrimitive::createQuad())
{
}

Elaina::CPostProcessPass::~CPostProcessPass()
{
	m_pQuadVAO.reset();
}

void Elaina::CPostProcessPass::renderV(
	const std::shared_ptr<CScene>& vScene, 
	const std::vector<std::shared_ptr<CFrameBuffer>>& vFrameBuffers, 
	const std::vector<size_t> vOutputIndices, size_t vIdxOfPasses)
{
	if (vIdxOfPasses == 0)
	{
		spdlog::warn("post process pass cannot first pass");
		return;
	}
	CRenderPass::renderV(vScene, vFrameBuffers, vOutputIndices, vIdxOfPasses);
	const auto& pLastFrameBuffer = vFrameBuffers[vOutputIndices[vIdxOfPasses - 1]];

	GL_SAFE_CALL(glDisable(GL_DEPTH_TEST));
	GL_SAFE_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	GL_SAFE_CALL(glActiveTexture(GL_TEXTURE0));
	pLastFrameBuffer->getAttachment(GL_COLOR_ATTACHMENT0)->bind();
	
	m_pShaderProgram->use();
	m_pShaderProgram->setUniform("uMainTex", 0);
	m_pQuadVAO->bind();
	m_pQuadVAO->draw();
}
