#pragma once

#include <TriEngine.h>

#include "Modules/SceneModule.h"
#include "Modules/DebugModule.h"
#include "Modules/FileMenu.h"

#include <imgui.h>

namespace FontType {
	enum Font {
		Regular = 0, Bold = 1, Italic = 2, BoldItalic = 3
	};
}

using namespace TriEngine;

class EditorLayer : public TriEngine::Layer
{
public:
	EditorLayer();
	virtual ~EditorLayer() = default;

	virtual void OnAttach() final;
	virtual void OnDetach() final;

	void OnUpdate(float deltaTime) final;
	virtual void OnImGuiRender() final;
	void UpdateTitleBar();
	void OnEvent(Event& e) final;
private:
	template<FontType::Font T>
	ImFont* GetFont() {
		ImGuiIO& io = ImGui::GetIO();
		return io.Fonts->Fonts[T];
	}

	template<FontType::Font T>
	void SetFont(ImFont* font) {
		ImGuiIO& io = ImGui::GetIO();
		auto& fontVector = io.Fonts->Fonts;
		if (T >= fontVector.Capacity) {
			fontVector.resize(T + 1);
		}
		fontVector[T] = font;
	}

	void SetupImGuiStyle();

	//Editor components
	SceneModule m_SceneModule;
	DebugModule m_DebugModule;
	FileMenu m_FileMenu;

	Reference<EditorCamera> m_Camera;

	glm::vec2 m_ViewPortSize;
	glm::vec2 m_PrevViewPortSize;

	Reference<Scene> m_ActiveScene;
	ImFont* m_EditorFont;
	bool m_SceneViewPaused = false;

};