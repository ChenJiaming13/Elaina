#pragma once

#include <unordered_set>
#include "CameraController.h"

namespace Elaina
{
	class CFPSController : public CCameraController
	{
	public:
		CFPSController();
		
		void control(const std::shared_ptr<CCamera>& vCamera) override;
		void update(float vDeltaTime) override;
		void onKeyDown(int vKey) override;
		void onKeyUp(int vKey) override;
		void onMouseButtonDown(int vKey) override;
		void onMouseButtonUp(int vKey) override;
		void onMouseMove(float vPosX, float vPosY) override;
		void onMouseScroll(float vOffsetX, float vOffsetY) override;

	private:
		void __updateCamera();
		float m_Yaw;
		float m_Pitch;
		float m_Speed;
		float m_Sensitivity;
		bool m_IsLeftBtnPressed;
		bool m_IsFirst;
		std::unordered_set<int> m_Keys;
	};
}
