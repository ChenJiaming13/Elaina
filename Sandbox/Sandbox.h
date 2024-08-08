#pragma once

#include <memory>
#include <vector>
#include "core/InputHandler.h"
#include "controller/ArcballController.h"

namespace Elaina
{
	struct SMaterial;
	struct SPbrMaterial;
	struct SPhongMaterial;
	struct SWaterMaterial;
	struct SCheckerMaterial;
	class CVertexArrayObject;
	class CDeferredLitPass;
	class CDirShadowMapPass;
	class CPointShadowMapPass;
	class CFrameBuffer;
	class CRenderPipeline;
	class CCameraController;
	class CScene;
	class CNode;
}

class CSandbox final : public Elaina::CInputHandler
{
public:
	void init(int vWidth, int vHeight);
	void render(float vCurrTime, float vDeltaTime);
	[[nodiscard]] const auto& getCameraController() const { return m_pCameraController; }

	void onWindowSizeChangeV(int vWidth, int vHeight) override;

private:
	void __setupScene(int vWidth, int vHeight);
	void __setupDeferredRenderPipeline(int vWidth, int vHeight);
	void __setupForwardRenderPipeline(int vWidth, int vHeight);
	void __setupMaterials();
	void __setupCamera(int vWidth, int vHeight);
	void __setupLights() const;
	void __setupNodes();
	void __renderUI();

	static std::shared_ptr<Elaina::CNode> __createNode(const std::shared_ptr<Elaina::CVertexArrayObject>& vVAO);
	static void __setMaterial(const std::shared_ptr<Elaina::CNode>& vRootNode, const std::shared_ptr<Elaina::SMaterial>& vMaterial);

	std::shared_ptr<Elaina::CScene> m_pScene;
	std::shared_ptr<Elaina::CRenderPipeline> m_pRenderPipeline;
	std::shared_ptr<Elaina::SPbrMaterial> m_pPlaneMat;
	std::shared_ptr<Elaina::SPbrMaterial> m_pObjMat;
	std::shared_ptr<Elaina::SPhongMaterial> m_pPhongMat;
	std::shared_ptr<Elaina::SCheckerMaterial> m_pCheckerMat;
	std::shared_ptr<Elaina::SWaterMaterial> m_pWaterMat;
	std::shared_ptr<Elaina::CDeferredLitPass> m_pDeferredLitPass;
	std::shared_ptr<Elaina::CDirShadowMapPass> m_pDirShadowMapPass;
	std::shared_ptr<Elaina::CPointShadowMapPass> m_pPointShadowMapPass;
	std::shared_ptr<Elaina::CCameraController> m_pCameraController;
	std::shared_ptr<Elaina::CNode> m_pObjNode;
	std::vector<std::shared_ptr<Elaina::CNode>> m_pNodes;
	size_t m_IndexOfNodes = 0;
	bool m_IsDeferredPipeline = true;
	std::array<std::string, 6> m_SkyBoxFiles;
};
