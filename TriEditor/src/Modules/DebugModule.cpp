#include "DebugModule.h"

#include "Core/Renderer/Renderer2D.h"
#include <imgui.h>

namespace TriEngine {
	void DebugModule::OnUpdate(float deltaTime)
	{
		m_FrameTime = deltaTime * 1000.0f;
	}

	void DebugModule::OnImGuiRender()
	{
		if (ImGui::Begin("Performance")) {
			auto stats = Renderer2D::GetStats();

			ImGui::Text("Renderering Stats:");
			ImGuiTableFlags flags = ImGuiTableFlags_Resizable;
			if (ImGui::BeginTable("perf_table", 2, flags)) {

				ImGui::TableNextColumn(); ImGui::Text("Frametime: %f", m_FrameTime);
				ImGui::Text("FPS: %d", static_cast<int32_t>(std::floor(1000.0f/std::floor(m_FrameTime))));

				ImGui::TableNextColumn(); ImGui::Text("Draw Calls: %d", stats.DrawCalls);
				ImGui::Text("Quads: %d", stats.QuadCount);
				ImGui::TableNextColumn(); ImGui::Text("Vertices: %d", stats.VertexCount());
				ImGui::Text("Indices: %d", stats.IndexCount());
			}
			ImGui::EndTable();

		}
		ImGui::End();

		m_Data->Sink->OnImGuiRender();

	}
}