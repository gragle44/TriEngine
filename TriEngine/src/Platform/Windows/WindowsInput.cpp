#include "tripch.h"
#include "Core/Base/Input.h"

#include "Base/Application.h"
#include <GLFW/glfw3.h>

namespace TriEngine {
	bool Input::IsKeyPressed(int keycode)
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		int state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS;
	}

	bool Input::IsMouseButtonPressed(int button)
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		int state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS;
	}

	glm::vec2 Input::GetMousePosition()
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);
		return { (float)xPos, (float)yPos };
	}

	bool Input::IsGamepadButtonPressed(int32_t joystick, int32_t button)
	{
		if (glfwJoystickIsGamepad(joystick))
		{
			GLFWgamepadstate state;

			if (glfwGetGamepadState(joystick, &state)) {
				return state.buttons[button] ? true : false;
			}
		}
		return false;
	}

	float Input::GetGamepadAxes(int32_t joystick, int32_t axis)
	{
		GLFWgamepadstate state;

		if (glfwGetGamepadState(joystick, &state)) {
			return state.axes[axis];
		}
	}

	float Input::GetMouseX()
	{
		return GetMousePosition().x;
	}

	float Input::GetMouseY()
	{
		return GetMousePosition().y;
	}

}