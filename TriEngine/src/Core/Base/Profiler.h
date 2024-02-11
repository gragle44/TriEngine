#pragma once

#include <chrono>

class Timer {
public:
	Timer() {
		Reset();
	}

	void Reset() {
		m_StartPoint = std::chrono::high_resolution_clock::now();
	}

	void Stop() {

	}

	float Ellapsed() {
		auto endPoint = std::chrono::high_resolution_clock::now();

		auto start = std::chrono::time_point_cast<std::chrono::nanoseconds>(m_StartPoint).time_since_epoch();
		auto end = std::chrono::time_point_cast<std::chrono::nanoseconds>(endPoint).time_since_epoch();

		return (end - start).count() * 0.001f * 0.001f * 0.001f;
	}

	float EllapsedMS() {
		return Ellapsed() * 1000.0f;
	}
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> m_StartPoint;
};