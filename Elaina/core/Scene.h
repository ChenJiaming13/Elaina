#pragma once

namespace Elaina
{
	class CNode;
	class CCamera;
	struct SDirectionalLight;
	class CScene
	{
	public:
		CScene() = default;
		~CScene();

		void setRootNode(const std::shared_ptr<CNode>& vRootNode) { m_pRootNode = vRootNode; }
		void setCamera(const std::shared_ptr<CCamera>& vCamera) { m_pCamera = vCamera; }
		void setDirectionalLight(const std::shared_ptr<SDirectionalLight>& vDirectionalLight) { m_pDirectionalLight = vDirectionalLight; }

		const auto& getRootNode() const { return m_pRootNode; }
		const auto& getCamera() const { return m_pCamera; }
		const auto& getDirectionalLight() const { return m_pDirectionalLight; }

	private:
		std::shared_ptr<CNode> m_pRootNode;
		std::shared_ptr<CCamera> m_pCamera;
		std::shared_ptr<SDirectionalLight> m_pDirectionalLight;
	};
}