#pragma once

#include <TriEngine.h>
#include "CommandHistory.h"

#include <imgui.h>

using namespace TriEngine;

struct EditorData {
	ImFont* EditorFont;
	Reference<Texture2D> PlayTexture;
	Reference<Texture2D> PauseTexture;
	Reference<Texture2D> FolderTexture;

	Reference<EditorCamera> Camera;
	glm::vec2 ViewPortSize;
	glm::vec2 PrevViewPortSize;

	Reference<Scene> EditorScene;
	Reference<Scene> ActiveScene;

	std::vector<Reference<Resource>> ModifiedResources;

	CommandHistory CmdHistory;

	bool NoProjectLoaded = true;

	bool SceneViewPaused = false;
	bool SceneRunning = false;

	float FrameTime;
};
