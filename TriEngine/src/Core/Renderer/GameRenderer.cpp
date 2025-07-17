#include "tripch.h"
#include "GameRenderer.h"

#include "Renderer2D.h"
#include "RenderCommand.h"
#include "Core/GameObjects/Components.h"
#include "Core/GameObjects/GameObject.h"
#include "Core/Projects/ProjectManager.h"
#include "Core/Base/Profiler.h"

#include "entt/entt.hpp"

namespace TriEngine {
    GameRenderer::GameRenderer()
    {
        //TODO: take in settings from application
        FrameBufferSettings fbSettings;
        fbSettings.Width = 1280;
        fbSettings.Height = 720;
        fbSettings.Samples = 1;
        fbSettings.Attachments = { RenderAttachmentType::Color, RenderAttachmentType::RedInteger, RenderAttachmentType::DepthStencil };

        Reference<FrameBuffer> mainFB = FrameBuffer::Create(fbSettings);

        m_MainRenderpass = std::make_shared<Renderpass>(mainFB);
        m_MainRenderpass->DepthTest = true;

		ParticleSystem::Init();
    }

    void GameRenderer::RenderScene(Scene* scene) {
		TRI_PROFILE;

		//2D rendering
		glm::mat4 cameraTransform;
		glm::mat4 cameraProjection;

		auto cameraView = scene->m_Registry.view<Transform2DComponent, Camera2DComponent>();

		for (auto entity : cameraView) {
			GameObject object = { entity, scene };
			auto [transform, camera] = cameraView.get<Transform2DComponent, Camera2DComponent>(entity);
			if (camera.Primary) {
				cameraTransform = transform.GetTransform();
				cameraProjection = camera.Camera.GetProjection();
			}
		}

		TRI_CORE_ASSERT(ProjectManager::IsProjectLoaded(), "No project is loaded");
		glm::vec4 clearColor = ProjectManager::GetCurrentProjectData().RenderingSettings.ClearColor;
		RenderCommand::SetClearColor(clearColor);

		Renderer2D::Begin(cameraProjection, cameraTransform, m_MainRenderpass);

		auto group = scene->m_Registry.group<Transform2DComponent>(entt::get<Sprite2DComponent>);

		for (auto entity : group) {
			auto [transform, sprite] = group.get<Transform2DComponent, Sprite2DComponent>(entity);

			bool empty = sprite.Texture->GetData().empty();
			if (!empty) {
				Renderer2D::SubmitQuad({ .Transform = transform.GetTransform(), .Tint = sprite.Tint, .Texture = sprite.Texture, .TilingFactor = sprite.TilingFactor, .Transparent = sprite.HasTransparency() });
			}
			else {
				Renderer2D::SubmitQuad(ColoredQuad(transform.GetTransform(), sprite.Tint, sprite.TilingFactor, sprite.HasTransparency()));
			}

		}
		Renderer2D::End();

		glm::mat4 viewProj = cameraProjection * glm::inverse(cameraTransform);

		ParticleSystem::Render(viewProj);

    }

    void GameRenderer::RenderSceneEditor(Scene* scene, const EditorCamera* editorCamera, std::optional<GameObject> selectedObject) {
		TRI_PROFILE;

		//2D rendering
		glm::mat4 cameraTransform;
		glm::mat4 cameraProjection;

		if (editorCamera != nullptr) {
			cameraTransform = editorCamera->GetTransform();
			cameraProjection = editorCamera->GetProjection();
		}
		else {
			auto cameraView = scene->m_Registry.view<Transform2DComponent, Camera2DComponent>();

			for (auto entity : cameraView) {
				auto [transform, camera] = cameraView.get<Transform2DComponent, Camera2DComponent>(entity);
				if (camera.Primary) {
					cameraTransform = transform.GetTransform();
					cameraProjection = camera.Camera.GetProjection();
				}
			}
		}

		glm::vec4 clearColor = { 0.15f, 0.15f, 0.15f, 1.0f };

		if (ProjectManager::IsProjectLoaded()) 
			clearColor = ProjectManager::GetCurrentProjectData().RenderingSettings.ClearColor;

		RenderCommand::SetClearColor(clearColor);

		m_MainRenderpass->Target->Bind();
		Renderer2D::Begin(cameraProjection, cameraTransform, m_MainRenderpass);

		auto group = scene->m_Registry.group<Transform2DComponent>(entt::get<Sprite2DComponent>);

		for (auto entity : group) {
			auto [transform, sprite] = group.get<Transform2DComponent, Sprite2DComponent>(entity);

			bool empty = sprite.Texture->GetData().empty();
			if (!empty) {
				Renderer2D::SubmitQuad({ .Transform = transform.GetTransform(), .Tint = sprite.Tint, .Texture = sprite.Texture, .TilingFactor = sprite.TilingFactor, .EntityId = (int32_t)entity, .Transparent = sprite.HasTransparency() });
			}
			else {
				Renderer2D::SubmitQuad(ColoredQuad(transform.GetTransform(), sprite.Tint, sprite.TilingFactor, (int32_t)entity, sprite.HasTransparency()));
			}

		}

		if (selectedObject && *selectedObject) {
			RenderCommand::SetLineWidth(3);

			auto& transform = selectedObject->GetComponent<Transform2DComponent>();

			glm::vec3 lineVertices[4];

			lineVertices[0] = {transform.Position.x - transform.Scale.x * 0.5f, transform.Position.y - transform.Scale.y * 0.5f, 1.0f};
			lineVertices[1] = {transform.Position.x + transform.Scale.x * 0.5f, transform.Position.y - transform.Scale.y * 0.5f, 1.0f};
			lineVertices[2] = {transform.Position.x - transform.Scale.x * 0.5f, transform.Position.y + transform.Scale.y * 0.5f, 1.0f};
			lineVertices[3] = {transform.Position.x + transform.Scale.x * 0.5f, transform.Position.y + transform.Scale.y * 0.5f, 1.0f};

			Renderer2D::SubmitLine({.Position1 = lineVertices[0], .Position2 = lineVertices[1]});
			Renderer2D::SubmitLine({.Position1 = lineVertices[1], .Position2 = lineVertices[3]});
			Renderer2D::SubmitLine({.Position1 = lineVertices[3], .Position2 = lineVertices[2]});
			Renderer2D::SubmitLine({.Position1 = lineVertices[2], .Position2 = lineVertices[0]});

			//Renderer2D::SubmitLine({.Position1 = {0.0f, 0.0f, 1.0f}, .Position2 = {3.0f, 3.0f, 1.0f}});
			//Renderer2D::SubmitLine({.Position1 = {-2.0f, 0.0f, 1.0f}, .Position2 = {5.0f, 3.0f, 1.0f}});
		}

		Renderer2D::End();

		glm::mat4 viewProj = cameraProjection * glm::inverse(cameraTransform);

		ParticleSystem::Render(viewProj);

		m_MainRenderpass->Target->UnBind();

    }

	void GameRenderer::SetViewportSize(uint32_t width, uint32_t height) {
		RenderCommand::SetViewPort({0, 0, width, height}); 
		m_MainRenderpass->Target->ReSize(width, height);
	}

}