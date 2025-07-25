#pragma once

#include <TriEngine.h>
#include "CommandHistory.h"

#include <imgui.h>

using namespace TriEngine;

enum class EditorState : uint8_t {
	Edit,
	Play
};

struct EditorData {
	std::shared_ptr<imgui_sink_mt> Sink;

	ImFont* EditorFont;
	Reference<Texture2D> PlayTexture;
	Reference<Texture2D> PauseTexture;
	Reference<Texture2D> FolderTexture;
	Reference<Texture2D> ReloadTexture;

	Reference<GameRenderer> Renderer;
	std::unique_ptr<EditorCamera> Camera;
	glm::vec2 ViewPortSize;
	glm::vec2 PrevViewPortSize;

	glm::vec2 ViewPortBoundsMin;
	glm::vec2 ViewPortBoundsMax;

	Reference<Scene> RestoreScene;
	Reference<Scene> ActiveScene;
	EditorState SceneCurrentState = EditorState::Edit;

	std::vector<Reference<Resource>> ModifiedResources;

	GameObject SelectedItem;
	GameObject RightSelectedItem;

	CommandHistory CmdHistory;

	bool NoProjectLoaded = true;

	bool ViewPortHovered = false;

	bool SceneViewPaused = false;
	bool SceneRunning = false;

	float FrameTime;
};
