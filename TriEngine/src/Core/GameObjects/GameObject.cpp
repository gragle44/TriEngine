#include "tripch.h"

#include "GameObject.h"

#include "Components.h"

namespace TriEngine {
    const std::string& GameObject::GetName() {
        return GetComponent<TagComponent>().Tag;
    }

    UUID GameObject::GetID() {
        return GetComponent<IDComponent>().ID;
    }

    GameObject GameObject::GetParent() {
        return m_Scene->GetObjectByID(GetComponent<RelationshipComponent>().Parent);
    }

    bool GameObject::HasParent() {
        return GetComponent<RelationshipComponent>().Parent;
    }
}