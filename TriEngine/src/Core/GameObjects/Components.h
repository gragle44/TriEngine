#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include "Core/Base/Core.h"
#include "Core/GameObjects/Script.h"
#include "Core/Renderer/Texture.h"
#include "Core/Renderer/ShaderStorageBuffer.h"
#include "Core/Renderer/OrthographicCamera.h"
#include "Core/Scripting/Script.h"

#include "entt/entt.hpp"
#include "box2d/b2_body.h"
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

	struct IDComponent {
		uint64_t ID = 0;

		IDComponent() = default;
		IDComponent(uint64_t id)
			:ID(id) {}
	};

	struct RigidBody2DComponent {
		enum class BodyType : uint8_t {
			Static,
			Dynamic, 
			Kinematic
		};
		BodyType Type = BodyType::Dynamic;

		void* Body;

		void ApplyVelocity(const glm::vec2& velocity) {
			b2Body* b2body = (b2Body*)Body;

			b2body->ApplyLinearImpulseToCenter({ velocity.x, velocity.y }, true);
		}

		void SetVelocity(const glm::vec2& velocity) {
			b2Body* b2body = (b2Body*)Body;

			b2body->SetLinearVelocity({ velocity.x, velocity.y });
		}

		void SetAngularVelocity(float velocity) {
			b2Body* b2body = (b2Body*)Body;

			b2body->SetAngularVelocity(velocity);
		}

		void SetPosition(const glm::vec2& pos) {
			b2Body* b2body = (b2Body*)Body;

			b2body->SetTransform({ pos.x, pos.y }, b2body->GetAngle());
		}

	};

	struct BoxCollider2DComponent {
		glm::vec2 Size = { 0.5f, 0.5f };
		float Density = 1.0f;
		float Friction = 0.3f;
		float Restitution = 0.0f;
		float RestitutionThreshold = 1.0f;
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

	struct ScriptComponent {
		std::shared_ptr<Script> ScriptInstance;

		std::string ScriptName;

		bool Active = true;

		ScriptComponent() = default;
		ScriptComponent(std::string_view name)
			:ScriptName(name) {}
		ScriptComponent(const ScriptComponent& other)
			:ScriptInstance(other.ScriptInstance), ScriptName(other.ScriptName), Active(other.Active) {}

	};

	struct NativeScriptComponent {
		std::unique_ptr<NativeScript> ScriptInstance;

		ScriptRegistry::FactoryFunction InstantiateScript;

		std::string ScriptName;

		bool ScriptActive = true;

		void Bind(const std::string& scriptName) {
			InstantiateScript = ScriptRegistry::GetFactoryFunction(scriptName);
			ScriptName = scriptName;
		}

		NativeScriptComponent() = default;

		NativeScriptComponent(const NativeScriptComponent& other)
			: ScriptInstance(nullptr), InstantiateScript(other.InstantiateScript),
			ScriptName(other.ScriptName), ScriptActive(other.ScriptActive) {}

		NativeScriptComponent& operator=(const NativeScriptComponent& other) {
			if (this != &other) {
				InstantiateScript = other.InstantiateScript;
				ScriptName = other.ScriptName;
				ScriptActive = other.ScriptActive;
			}
			return *this;
		}
	};


	struct Camera2DComponent {
		OrthographicCamera Camera;
		bool Primary = false;
		bool Resizeable = true;

		Camera2DComponent() = default;
		Camera2DComponent(const Camera2DComponent&) = default;
	};

	struct Sprite2DComponent {
		Reference<Texture2D> Texture;
		glm::vec4 Tint = { 1.0f, 1.0f, 1.0f, 1.0f };
		float TilingFactor = 1.0f;

		bool HasTransparency() { return Texture->HasTransparency() || Tint.a < 1.0f; }

		Sprite2DComponent() 
			:Texture(Texture2D::Create(glm::ivec2(1, 1))) {}
		Sprite2DComponent(Reference<Texture2D> texture2D)
			:Texture(texture2D) {}
		Sprite2DComponent(const Sprite2DComponent&) = default;
	};

	struct ParticleEmmiterComponent {
		glm::vec4 MinColor = { 1.0f, 1.0f, 1.0f, 1.0f }, MaxColor = { 1.0f, 1.0f, 1.0f, 1.0f };
		glm::vec3 MinOffset, MaxOffset;
		glm::vec2 MinVelocity, MaxVelocity;
		glm::vec2 MinAccel, MaxAccel;
		float MinLife = 1.0f, MaxLife = 1.0f;

		float SpawnInterval = 0.5, SpawnTimer;

		Reference<Texture2D> Texture = nullptr;

		ParticleEmmiterComponent() = default;
		ParticleEmmiterComponent(const ParticleEmmiterComponent&) = default;
	};
}