#pragma once

#include "Framebuffer.h"
#include "Core/GameObjects/Scene.h"
#include "Core/GameObjects/EditorCamera.h"
#include "Particle.h"

namespace TriEngine {

	struct GameRendererData {
		Reference<EditorCamera> editorCamera;
		Reference<Renderpass> MainRenderpass;
		glm::vec2 ViewportSize;
	};

	class GameRenderer {
	public:
		GameRenderer();
		~GameRenderer() = default;

		void RenderScene(Scene* scene);
		void RenderSceneEditor(Scene* scene, const EditorCamera* editorCamera, std::optional<GameObject> selectedObject);

		void SetViewportSize(uint32_t width, uint32_t height) { m_MainRenderpass->Target->ReSize(width, height); }

		const Reference<FrameBuffer> GetFinalFramebuffer() { return m_MainRenderpass->Target; }

	private:
		glm::vec2 m_ViewportSize;
		Reference<Renderpass> m_MainRenderpass;
	};
}