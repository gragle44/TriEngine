#pragma once

#include "Core.h"
#include "KeyCodes.h"
#include "ControllerCodes.h"
#include "MouseButtonCodes.h"
#include <glm/vec2.hpp>

namespace TriEngine {
	class Input 
	{
	public:
		static bool IsKeyPressed(int keycode);
		
		static bool IsMouseButtonPressed(int button);
		static float GetMouseX();
		static float GetMouseY();
		static glm::vec2 GetMousePosition();

		static bool IsGamepadButtonPressed(int32_t joystick, int32_t button);
		static float GetGamepadAxes(int32_t joystick, int32_t axis);
	};
}