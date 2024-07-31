#include "pch.h"
#include "ForwardPbrPass.h"
#include "base/ShaderProgram.h"
#include "core/Node.h"
#include "core/Scene.h"
#include "core/Mesh.h"
#include "core/Camera.h"
#include "light/Light.h"
#include "safe.h"

void Elaina::CForwardPbrPass::renderV(
	const std::shared_ptr<CScene>& vScene, 
	const std::vector<std::shared_ptr<CFrameBuffer>>& vFrameBuffers, 
	const std::vector<size_t> vOutputIndices, size_t vIdxOfPasses)
{
	CRenderPass::renderV(vScene, vFrameBuffers, vOutputIndices, vIdxOfPasses);
	GL_SAFE_CALL(glEnable(GL_DEPTH_TEST));
	GL_SAFE_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	CNode::traverse(vScene->getRootNode(), [&](const std::shared_ptr<Elaina::CNode>& vNode) {
		const auto& pCamera = vScene->getCamera();
		const auto& pDirLight = vScene->getDirectionalLight();
		for (const auto& pMesh : vNode->getMeshes())
		{
			const auto& pProgram = pMesh->getShaderProgram();
			pProgram->setUniform("uModel", vNode->getModelMatrix());
			pProgram->setUniform("uView", pCamera->getViewMatrix());
			pProgram->setUniform("uProjection", pCamera->getProjectionMatrix());
			pProgram->setUniform("uCamPos", pCamera->getWorldPos());
			pProgram->setUniform("uLightPosition", pDirLight->_LightPos);
			pProgram->setUniform("uLightColor", pDirLight->_LightColor);
			pProgram->autoUse();
			pMesh->draw();
		}
	});
}