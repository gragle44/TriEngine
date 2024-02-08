#pragma once

#include "Core/Base/Layer.h"

#include "Core/Events/ApplicationEvent.h"
#include "Core/Events/KeyEvent.h"
#include "Core/Events/MouseEvent.h"

namespace TriEngine {
	class  ImGuiLayer : public Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() final;
		virtual void OnDetach() final;
		virtual void OnImGuiRender() final;
		virtual void OnEvent(Event& e) final;

		void Begin();
		void End();
	private:
		float m_Time = 0.0f;
	};
}