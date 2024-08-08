#include "pch.h"
#include "PostProcessPass.h"
#include "base/Framebuffer.h"
#include "base/Texture2D.h"
#include "base/ShaderProgram.h"
#include "base/VertexArrayObject.h"
#include "primitive/Primitive.h"
#include "safe.h"

Elaina::CPostProcessPass::CPostProcessPass(const std::shared_ptr<CShaderProgram>& vShaderProgram, bool vIsFinalPass) :
	m_pQuadVAO(CPrimitive::createQuad()), m_pShaderProgram(vShaderProgram), m_IsFinalPass(vIsFinalPass)
{
}

void Elaina::CPostProcessPass::renderV(const std::shared_ptr<CScene>& vScene)
{
	m_pFrameBuffer->bind();
	GL_SAFE_CALL(glViewport(0, 0, m_pFrameBuffer->getWidth(), m_pFrameBuffer->getHeight()));
	GL_SAFE_CALL(glDisable(GL_DEPTH_TEST));
	GL_SAFE_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	GL_SAFE_CALL(glActiveTexture(GL_TEXTURE0));
	m_pLastFrameBuffer->getAttachment(GL_COLOR_ATTACHMENT0)->bind();
	
	m_pShaderProgram->use();
	m_pShaderProgram->setUniform("uMainTex", 0);
	m_pQuadVAO->bind();
	m_pQuadVAO->draw();
}

bool Elaina::CPostProcessPass::validateV() const
{
	if (m_pShaderProgram == nullptr)
	{
		spdlog::error("[PostProcessPass] shader program miss");
		return false;
	}
	return true;
}

void Elaina::CPostProcessPass::initV(int vWidth, int vHeight)
{
	// TODO: save memory when has many post-process pass
	if (m_IsFinalPass)
		m_pFrameBuffer = CFrameBuffer::getDefaultFrameBuffer();
	else
		m_pFrameBuffer = CFrameBuffer::createFrameBuffer(vWidth, vHeight, std::vector{ 3 });
}

void Elaina::CPostProcessPass::onWindowSizeChangeV(int vWidth, int vHeight)
{
	m_pFrameBuffer->resize(vWidth, vHeight);
}
