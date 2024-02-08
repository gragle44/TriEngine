#pragma once

#include <TriEngine.h>

#include "imgui.h"

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
	void OnEvent(Event& e) final;
private:
	void SetupImGuiStyle();

	Reference<Texture2D> m_Texture;
	Reference<FrameBuffer> m_FrameBuffer;
	glm::vec2 m_ViewPortSize;

	OrthographicCameraController m_CameraController;

	ImFont* m_EditorFont;
	bool m_SceneViewPaused = false;

};