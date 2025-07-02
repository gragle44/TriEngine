#include "tripch.h"
#include "GameRenderer.h"

#include "Renderer2D.h"
#include "Core/GameObjects/Components.h"
#include "Core/GameObjects/GameObject.h"
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

    void GameRenderer::RenderSceneEditor(Scene* scene, const EditorCamera* editorCamera) {
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

		Renderer2D::End();

;		glm::mat4 viewProj = cameraProjection * glm::inverse(cameraTransform);

		ParticleSystem::Render(viewProj);

		m_MainRenderpass->Target->UnBind();

    }

}