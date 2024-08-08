#pragma once

namespace Elaina
{
	class CInputHandler
	{
	public:
		virtual ~CInputHandler() = default;

		virtual void onWindowSizeChangeV(int vWidth, int vHeight) {}
		virtual void onKeyDownV(int vKey) {}
		virtual void onKeyUpV(int vKey) {}
		virtual void onMouseButtonDownV(int vKey) {}
		virtual void onMouseButtonUpV(int vKey) {}
		virtual void onMouseMoveV(float vPosX, float vPosY) {}
		virtual void onMouseScrollV(float vOffsetX, float vOffsetY) {}
	};

	constexpr auto HIVE_MOUSE_BUTTON_LEFT = 0;
	constexpr auto HIVE_MOUSE_BUTTON_RIGHT = 1;
	constexpr auto HIVE_MOUSE_BUTTON_MIDDLE = 2;
	constexpr auto HIVE_MOUSE_BUTTON_BACK = 3;
	constexpr auto HIVE_MOUSE_BUTTON_FORWARD = 4;
	constexpr auto HIVE_KEY_W = 87;
	constexpr auto HIVE_KEY_S = 83;
	constexpr auto HIVE_KEY_A = 65;
	constexpr auto HIVE_KEY_D = 68;
	constexpr auto HIVE_KEY_Q = 81;
	constexpr auto HIVE_KEY_E = 69;
	constexpr auto HIVE_KEY_Z = 90;
}