#include "pch.h"
#include "DeferredGeoPass.h"
#include "base/Framebuffer.h"
#include "base/ShaderProgram.h"
#include "core/Camera.h"
#include "core/Scene.h"
#include "core/Node.h"
#include "core/Mesh.h"
#include "safe.h"

Elaina::CDeferredGeoPass::CDeferredGeoPass(const std::shared_ptr<CShaderProgram>& vShaderProgram) :m_pShaderProgram(vShaderProgram)
{
}

Elaina::CDeferredGeoPass::~CDeferredGeoPass()
{
	m_pShaderProgram.reset();
}

void Elaina::CDeferredGeoPass::renderV(const std::shared_ptr<CScene>& vScene, const std::vector<std::shared_ptr<CFrameBuffer>>& vFrameBuffers, const std::vector<size_t> vOutputIndices, size_t vIdxOfPasses)
{
	CRenderPass::renderV(vScene, vFrameBuffers, vOutputIndices, vIdxOfPasses);
	const auto& pCamera = vScene->getCamera();
	const auto& pLight = vScene->getDirectionalLight();
	GL_SAFE_CALL(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
	GL_SAFE_CALL(glEnable(GL_DEPTH_TEST));
	GL_SAFE_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	m_pShaderProgram->use();
	m_pShaderProgram->setUniform("uView", pCamera->getViewMatrix());
	m_pShaderProgram->setUniform("uProjection", pCamera->getProjectionMatrix());
	CNode::traverse(vScene->getRootNode(), [this](const std::shared_ptr<CNode>& vNode) {
		m_pShaderProgram->setUniform("uModel", vNode->getModelMatrix());
		for (const auto& pModel : vNode->getMeshes())
		{
			pModel->draw();
		}
	});
}
