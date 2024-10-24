#include "pch.h"
#include "Scene.h"

Elaina::CScene::~CScene()
{
	m_pDirectionalLight.reset();
	m_pCamera.reset();
	m_pRootNode.reset();
}
