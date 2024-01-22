#pragma once

#include "Utils/PlatformUtils.h"

namespace TriEngine {
	class DeltaTime {
	public:
		DeltaTime() = default;

		operator float() const { return m_Time; }
		operator double() const { return (double)m_Time; }

		void Update() {
			float currentTime = Time::GetTime();
			m_Time = currentTime - m_LastTime;
			m_LastTime = currentTime;
		}

		float GetS() const { return m_Time; }
		float GetMS() const { return m_Time * 1000.0f; }

	private:
		float m_Time = 0.0f;
		float m_LastTime = 0.0f;
	};
}
