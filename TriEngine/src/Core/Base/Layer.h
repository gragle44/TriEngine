#pragma once

#include "Core.h"
#include "Core/Events/Event.h"

namespace TriEngine {
	class Layer 
	{
	public:
		Layer() = default;
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(float deltaTime) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}
	};
}