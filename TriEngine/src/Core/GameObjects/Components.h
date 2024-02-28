#pragma once

#include "Core/Base/Core.h"
#include "Core/Renderer/Texture.h"
#include "Core/Renderer/OrthographicCamera.h"
#include "entt/entt.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace TriEngine {
	struct TagComponent {
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			:Tag(tag) {}
	};

	struct Transform2DComponent {
		glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
		float Rotation = 0.0f;
		glm::vec2 Scale = { 1.0f, 1.0f};

		Transform2DComponent() = default;
		Transform2DComponent(const Transform2DComponent&) = default;
		Transform2DComponent(const glm::vec3& position)
			: Position(position) {}

		glm::mat4 GetTransform() const {
			return glm::translate(glm::mat4(1.0f), Position)
				* glm::rotate(glm::mat4(1.0f), glm::radians(Rotation), { 0, 0, 1 })
				* glm::scale(glm::mat4(1.0f), {Scale.x, Scale.y, 1.0f});
		}
	};

	struct TransformComponent {
		glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& position)
			: Position(position) {}

		glm::mat4 GetTransform() const {
			return glm::translate(glm::mat4(1.0f), Position) 
				* glm::toMat4(glm::quat(Rotation)) 
				* glm::scale(glm::mat4(1.0f), Scale);
		}

	};

	class Script;

	struct ScriptComponent {
		std::unique_ptr<Script> ScriptInstance;

		std::unique_ptr<Script>(*InstantiateScript)();

		template<typename T>
		void Bind() {
			InstantiateScript = []() -> std::unique_ptr<Script> {return std::make_unique<T>(); };
		}
	};


	struct Camera2DComponent {
		OrthographicCamera Camera;
		bool Resizeable = true;

		Camera2DComponent() = default;
		Camera2DComponent(const Camera2DComponent&) = default;
	};

	struct Sprite2DComponent {
		Reference<Texture2D> Texture;
		glm::vec4 Tint = { 1.0f, 1.0f, 1.0f, 1.0f };
		float TilingFactor = 1.0f;

		Sprite2DComponent() = default;
		Sprite2DComponent(Reference<Texture2D> texture2d)
			:Texture(texture2d) {}
		Sprite2DComponent(const Sprite2DComponent&) = default;
	};
}