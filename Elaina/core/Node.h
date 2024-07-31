#pragma once

#include <vector>
#include <string>
#include <functional>
#include "core/Transform.h"

namespace Elaina
{
	class CMesh;
	class CNode
	{
	public:
		CNode();
		
		glm::mat4 getModelMatrix() const { return m_ModelMatrix; }
		const std::vector<std::shared_ptr<CNode>>& getChildNodes() const { return m_ChildNodes; }
		void clearChilds() { m_ChildNodes.clear(); }
		void addChild(const std::shared_ptr<CNode>& vNode);
		void setPosition(const glm::vec3& vPosition);
		void setRotation(const glm::vec3& vRotation);
		void setScale(const glm::vec3& vScale);
		const CTransform& getTransform() const { return m_Transform; }
		void addMesh(const std::shared_ptr<CMesh>& vMesh);
		const auto& getMeshes() const { return m_Meshes; }
		static void traverse(const std::shared_ptr<CNode>& vRootNode, std::function<void(const std::shared_ptr<CNode>&)> vFuncTraverse);
	
	private:
		void __updateModelMatrix();
		CTransform m_Transform;
		glm::mat4 m_ModelMatrix;
		CNode* m_ParentNode; // dangerous
		std::vector<std::shared_ptr<CMesh>> m_Meshes;
		std::vector<std::shared_ptr<CNode>> m_ChildNodes;
	};
}