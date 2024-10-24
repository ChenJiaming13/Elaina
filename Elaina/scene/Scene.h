#pragma once

namespace Elaina
{
	class CNode;
	class CCamera;
	struct SDirectionalLight;
	struct SPointLight;
	class CScene
	{
	public:
		CScene() = default;
		~CScene();

		void setRootNode(const std::shared_ptr<CNode>& vRootNode) { m_pRootNode = vRootNode; }
		void setCamera(const std::shared_ptr<CCamera>& vCamera) { m_pCamera = vCamera; }
		void setDirectionalLight(const std::shared_ptr<SDirectionalLight>& vDirectionalLight) { m_pDirectionalLight = vDirectionalLight; }
		void setPointLight(const std::shared_ptr<SPointLight>& vPointLight) { m_PointLight = vPointLight; }

		[[nodiscard]] const auto& getRootNode() const { return m_pRootNode; }
		[[nodiscard]] const auto& getCamera() const { return m_pCamera; }
		[[nodiscard]] const auto& getDirectionalLight() const { return m_pDirectionalLight; }
		[[nodiscard]] const auto& getPointLight() const { return m_PointLight; }

	private:
		std::shared_ptr<CNode> m_pRootNode;
		std::shared_ptr<CCamera> m_pCamera;
		std::shared_ptr<SDirectionalLight> m_pDirectionalLight;
		std::shared_ptr<SPointLight> m_PointLight;
	};
}