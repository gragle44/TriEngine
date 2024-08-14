#pragma once

#include "tripch.h"
#include "Event.h"

namespace TriEngine {
	class JoystickConnectedEvent : public Event {
	public:
		enum class JoyStickConnectedEventType : uint8_t {
			Connected = 0,
			Disconected = 1
		};

		JoystickConnectedEvent(int joystick, JoyStickConnectedEventType connected)
			: m_JoystickID(joystick), m_Connected(connected) {}

		int GetJoystick() const { return m_JoystickID; }
		JoyStickConnectedEventType GetConnectionEventType() const { return m_Connected; }

		EVENT_CLASS_CATEGORY(EventCategoryJoystick | EventCategoryInput)
		EVENT_CLASS_TYPE(JoyStickConnected)

	private:
		int m_JoystickID;
		JoyStickConnectedEventType m_Connected;
	};
}
