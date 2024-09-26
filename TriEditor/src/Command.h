#pragma once

#include "Core/GameObjects/Components.h"

using namespace TriEngine;

class Command {
public:
	virtual ~Command() {}
	virtual void Execute() = 0;
	virtual void Undo() = 0;
};

class AddGameObjectCommand : public Command {
public:
	AddGameObjectCommand(Reference<Scene> scene)
		:m_Scene(scene), m_ID(UUID()), m_Name("Object") {}

	virtual void Execute() override {
		m_Object = m_Scene->CreateGameObjectUUID(m_ID, m_Name);
	}

	virtual void Undo() override {
		if (m_Scene->IsObjectValid(m_Object))
			m_Scene->DeleteGameObject(m_Object);
	}

private:
	//TODO: make this a weak ref or scene id
	Reference<Scene> m_Scene;
	UUID m_ID;
	std::string m_Name;
	GameObject m_Object;
};

class DeleteGameObjectCommand : public Command {
public:
	DeleteGameObjectCommand(Reference<Scene> scene, GameObject object) {
	}
private:
	Reference<Scene> m_Scene;
	GameObject m_Object;

};

class TransformComponentEditCommand : public Command {
public:
	TransformComponentEditCommand(GameObject object, glm::vec3 position, float rotation, glm::vec2 scale)
		:m_Object(object), m_Position(position), m_Rotation(rotation), m_Scale(scale) {}

	virtual void Execute() override {
		auto& transform = m_Object.GetComponent<Transform2DComponent>();

		m_OldComponent = transform;

		transform.Position = m_Position;
		transform.Rotation = m_Rotation;
		transform.Scale = m_Scale;
	}

	virtual void Undo() override {
		auto& transform = m_Object.GetComponent<Transform2DComponent>();

		transform = m_OldComponent;
	}

private:
	GameObject m_Object;

	Transform2DComponent m_OldComponent;

	glm::vec3 m_Position;
	float m_Rotation;
	glm::vec2 m_Scale;
};