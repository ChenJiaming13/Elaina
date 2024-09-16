#pragma once

#include "core/InputHandler.h"
#include "core/Camera.h"
#include <memory>

namespace Elaina
{
	class CCamera;
	class CCameraController : public CInputHandler
	{
	public:
		virtual void initV() {}
		virtual void controlV(const std::shared_ptr<CCamera>& vCamera) { m_pCamera = vCamera; }
		virtual void updateV(float vDeltaTime) {}
		void onWindowSizeChangeV(int vWidth, int vHeight) override { m_pCamera->setViewport(vWidth, vHeight); }

	protected:
		std::shared_ptr<CCamera> m_pCamera;
	};
}