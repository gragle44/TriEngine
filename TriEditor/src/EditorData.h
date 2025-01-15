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
	ImFont* EditorFont;
	Reference<Texture2D> PlayTexture;
	Reference<Texture2D> PauseTexture;
	Reference<Texture2D> FolderTexture;

	Reference<GameRenderer> Renderer;
	Reference<EditorCamera> Camera;
	glm::vec2 ViewPortSize;
	glm::vec2 PrevViewPortSize;

	glm::vec2 ViewPortBoundsMin;
	glm::vec2 ViewPortBoundsMax;

	Reference<Scene> EditorScene;
	Reference<Scene> ActiveScene;
	EditorState SceneCurrentState = EditorState::Edit;

	std::vector<Reference<Resource>> ModifiedResources;

	GameObject SelectedItem;
	GameObject RightSelectedItem;

	CommandHistory CmdHistory;

	bool NoProjectLoaded = true;

	bool SceneViewPaused = false;
	bool SceneRunning = false;

	float FrameTime;
};
