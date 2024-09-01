#pragma once

#include <unordered_set>
#include "CameraController.h"

namespace Elaina
{
	class CFPSController final : public CCameraController
	{
	public:
		CFPSController();
		
		void control(const std::shared_ptr<CCamera>& vCamera) override;
		void update(float vDeltaTime) override;
		void onKeyDownV(int vKey) override;
		void onKeyUpV(int vKey) override;
		void onMouseButtonDownV(int vKey) override;
		void onMouseButtonUpV(int vKey) override;
		void onMouseMoveV(float vPosX, float vPosY) override;
		void onMouseScrollV(float vOffsetX, float vOffsetY) override;

	private:
		void __updateCameraFront() const;
		float m_Yaw;
		float m_Pitch;
		float m_Speed;
		float m_Sensitivity;
		bool m_IsLeftBtnPressed;
		bool m_IsFirst;
		std::unordered_set<int> m_Keys;
	};
}
