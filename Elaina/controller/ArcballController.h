#pragma once

#include "CameraController.h"

namespace Elaina
{
	class CArcballController : public CCameraController
	{
	public:
		CArcballController();
		void control(const std::shared_ptr<CCamera>& vCamera) override;
		void onMouseButtonDown(int vKey) override;
		void onMouseButtonUp(int vKey) override;
		void onMouseMove(float vPosX, float vPosY) override;
		void onMouseScroll(float vOffsetX, float vOffsetY) override;
	
	private:
		void __updateCamera();
		glm::vec3 m_TargetPos;
		float m_Distance;
		float m_Yaw;
		float m_Pitch;
		float m_Sensitivity;
		bool m_IsLeftBtnPressed;
		bool m_IsRightBtnPressed;
		bool m_IsFirst;
	};
}