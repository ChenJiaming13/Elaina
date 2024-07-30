#include "pch.h"
#include "ArcballController.h"

Elaina::CArcballController::CArcballController() :
	m_TargetPos(0.0f, 0.0f, 0.0f), m_Distance(5.0f),
	m_Yaw(-90.0f), m_Pitch(0.0f), m_Sensitivity(1.0f),
	m_IsLeftBtnPressed(false), m_IsRightBtnPressed(false), m_IsFirst(true)
{
}

void Elaina::CArcballController::control(const std::shared_ptr<CCamera>& vCamera)
{
	CCameraController::control(vCamera);
	__updateCamera();
}

void Elaina::CArcballController::onMouseButtonDown(int vKey)
{
	if (vKey == HIVE_MOUSE_BUTTON_LEFT) m_IsLeftBtnPressed = true;
	else if (vKey == HIVE_MOUSE_BUTTON_RIGHT) m_IsRightBtnPressed = true;
}

void Elaina::CArcballController::onMouseButtonUp(int vKey)
{
	if (vKey == HIVE_MOUSE_BUTTON_LEFT)
	{
		m_IsLeftBtnPressed = false;
		m_IsFirst = true;
	}
	else if (vKey == HIVE_MOUSE_BUTTON_RIGHT)
	{
		m_IsRightBtnPressed = false;
		m_IsFirst = true;
	}
}

void Elaina::CArcballController::onMouseMove(float vPosX, float vPosY)
{
	if (!m_IsLeftBtnPressed && !m_IsRightBtnPressed) return;
	static float LastPosX, LastPosY;
	if (m_IsFirst)
	{
		LastPosX = vPosX;
		LastPosY = vPosY;
		m_IsFirst = false;
		return;
	}
	float DeltaX = vPosX - LastPosX;
	float DeltaY = vPosY - LastPosY;
	LastPosX = vPosX;
	LastPosY = vPosY;
	if (m_IsLeftBtnPressed) {
		m_Yaw += DeltaX * m_Sensitivity;
		m_Pitch -= DeltaY * m_Sensitivity;
		if (m_Pitch > 89.0f) m_Pitch = 89.0f;
		if (m_Pitch < -89.0f) m_Pitch = -89.0f;
		__updateCamera();
	}
	if (m_IsRightBtnPressed) {
		DeltaX *= 0.005f * m_Sensitivity;
		DeltaY *= 0.005f * m_Sensitivity;
		glm::vec3 DeltaPos = DeltaX * m_pCamera->getRight() - DeltaY * m_pCamera->getUp();
		glm::vec3 Position = m_pCamera->getWorldPos();
		m_pCamera->setWorldPos(Position - DeltaPos);
		m_TargetPos -= DeltaPos;
	}
}

void Elaina::CArcballController::onMouseScroll(float vOffsetX, float vOffsetY)
{
	if (m_pCamera->getCameraType() == CCamera::ECameraType::PERSP)
	{
		m_Distance -= vOffsetY * m_Sensitivity * 0.1f;
		m_Distance = glm::max(0.0f, m_Distance);
		m_pCamera->setWorldPos(m_TargetPos - m_pCamera->getFront() * m_Distance);
	}
	else if (m_pCamera->getCameraType() == CCamera::ECameraType::ORTHO)
	{
		float NearHeight = m_pCamera->getNearHeight();
		NearHeight -= vOffsetY * m_Sensitivity * 3.0f;
		NearHeight = glm::max(1e-6f, NearHeight);
		m_pCamera->setNearHeight(NearHeight);
	}
}

void Elaina::CArcballController::__updateCamera()
{
	glm::vec3 Front{};
	Front.x = glm::cos(glm::radians(m_Yaw)) * glm::cos(glm::radians(m_Pitch));
	Front.y = glm::sin(glm::radians(m_Pitch));
	Front.z = glm::sin(glm::radians(m_Yaw)) * glm::cos(glm::radians(m_Pitch));
	Front = glm::normalize(Front);
	m_pCamera->setFront(Front);
	m_pCamera->setWorldPos(m_TargetPos - m_pCamera->getFront() * m_Distance);
}