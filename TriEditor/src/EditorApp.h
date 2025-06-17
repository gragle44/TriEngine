#pragma once

#include "EditorData.h"

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
	void OnRender(float deltaTime);
	virtual void OnImGuiRender() final;
	void UpdateTitleBar();
	void OnEvent(Event& e) final;
	bool OnKeyPressed(KeyPressedEvent& e);
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

	void RenderPlaybuttons();
	void SetupImGuiStyle();

	void LoadEmptyScene();

	void PromptLoadProject();
	void LoadProject(const std::string& path);
	void SaveProject(const std::string& path);

	void LoadScene(const std::string& path);
	void LoadScene(ResourceID id);
	void SaveScene(const std::string& path);

	void StartScene();
	void StopScene();

	EditorData* m_Data;

	//Editor components
	SceneModule m_SceneModule;
	DebugModule m_DebugModule;
	FileMenu m_FileMenu;
};
