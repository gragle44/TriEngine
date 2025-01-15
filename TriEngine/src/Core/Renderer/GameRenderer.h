#pragma once

#include "Framebuffer.h"
#include "Core/GameObjects/Scene.h"
#include "Core/GameObjects/EditorCamera.h"

namespace TriEngine {

	struct GameRendererData {
		Reference<EditorCamera> EditorCamera;
		Reference<Renderpass> MainRenderpass;
		glm::vec2 ViewportSize;
	};

	class GameRenderer {
	public:
		GameRenderer();
		~GameRenderer() = default;

		void RenderScene(Scene* scene);
		void RenderSceneEditor(Scene* scene);

		void SetViewportSize(uint32_t width, uint32_t height) { m_MainRenderpass->Target->ReSize(width, height); }

		void SetEditorCamera(Reference<EditorCamera> camera) { m_EditorCamera = camera; }

		const Reference<FrameBuffer> GetFinalFramebuffer() { return m_MainRenderpass->Target; }

	private:
		Reference<EditorCamera> m_EditorCamera;
		glm::vec2 m_ViewportSize;
		Reference<Renderpass> m_MainRenderpass;
	};
}