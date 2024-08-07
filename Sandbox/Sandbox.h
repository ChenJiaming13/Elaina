#pragma once

#include <memory>
#include <vector>
#include "core/InputHandler.h"
#include "controller/ArcballController.h"

namespace Elaina
{
	struct SCheckerMaterial;
}

namespace Elaina
{
	struct SMaterial;
	struct SPbrMaterial;
	struct SPhongMaterial;
	class CVertexArrayObject;
	class CDeferredLitPass;
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

	void onWindowSizeChange(int vWidth, int vHeight) override;

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

	std::shared_ptr<Elaina::CScene> m_Scene;
	std::shared_ptr<Elaina::CRenderPipeline> m_RenderPipeline;
	std::shared_ptr<Elaina::SPbrMaterial> m_PlaneMat;
	std::shared_ptr<Elaina::SPbrMaterial> m_ObjMat;
	std::shared_ptr<Elaina::SPhongMaterial> m_PhongMat;
	std::shared_ptr<Elaina::SCheckerMaterial> m_CheckerMat;
	std::shared_ptr<Elaina::CFrameBuffer> m_DirShadowMapFB;
	std::shared_ptr<Elaina::CFrameBuffer> m_PointShadowMapFB;
	std::shared_ptr<Elaina::CDeferredLitPass> m_DeferredLitPass;
	std::shared_ptr<Elaina::CCameraController> m_pCameraController;
	std::shared_ptr<Elaina::CNode> m_pObjNode;
	std::vector<std::shared_ptr<Elaina::CNode>> m_pNodes;
	size_t m_IndexOfNodes = 0;
	bool m_IsDeferredPipeline = true;
};
