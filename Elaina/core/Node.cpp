#include "pch.h"
#include "Node.h"
#include "Mesh.h"
#include <queue>

Elaina::CNode::CNode()
	:m_Transform{}, m_ChildNodes{}, m_ParentNode{ nullptr }, m_ModelMatrix{}, m_Meshes{}
{
	__updateModelMatrix();
}

void Elaina::CNode::addChild(const std::shared_ptr<CNode>& vNode)
{
	m_ChildNodes.push_back(vNode);
	vNode->m_ParentNode = this;
	vNode->__updateModelMatrix();
}

void Elaina::CNode::setPosition(const glm::vec3& vPosition)
{
	m_Transform.setLocalPosition(vPosition);
	__updateModelMatrix();
}

void Elaina::CNode::setRotation(const glm::vec3& vRotation)
{
	m_Transform.setLocalRotation(vRotation);
	__updateModelMatrix();
}

void Elaina::CNode::setScale(const glm::vec3& vScale)
{
	m_Transform.setLocalScale(vScale);
	__updateModelMatrix();
}

void Elaina::CNode::addMesh(const std::shared_ptr<CMesh>& vMesh)
{
	m_Meshes.push_back(vMesh);
}

void Elaina::CNode::traverse(const std::shared_ptr<CNode>& vRootNode, std::function<void(const std::shared_ptr<CNode>&)> vFuncTraverse)
{
	std::queue<std::shared_ptr<CNode>> QueueNodes;
	QueueNodes.push(vRootNode);
	while (!QueueNodes.empty())
	{
		const auto& pCurrNode = QueueNodes.front();
		vFuncTraverse(pCurrNode);
		QueueNodes.pop();
		for (const auto& pNode : pCurrNode->m_ChildNodes)
			QueueNodes.push(pNode);
	}
}

void Elaina::CNode::__updateModelMatrix()
{
	if (m_ParentNode == nullptr) m_ModelMatrix = m_Transform.getLocalModelMatrix();
	else m_ModelMatrix = m_ParentNode->getModelMatrix() * m_Transform.getLocalModelMatrix();
	for (const auto& pChild : m_ChildNodes)
	{
		pChild->__updateModelMatrix();
	}
}