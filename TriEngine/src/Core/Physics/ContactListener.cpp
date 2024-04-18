#include "tripch.h"
#include "ContactListener.h"

#include "Core/GameObjects/GameObject.h"
#include "Core/GameObjects/Components.h"

#include "box2d/b2_contact.h"

namespace TriEngine {
	void TriEngine::ContactListener::BeginContact(b2Contact* contact)
	{
		b2Body* bodyA = contact->GetFixtureA()->GetBody();
		b2Body* bodyB = contact->GetFixtureB()->GetBody();

		if (bodyA->GetUserData().pointer != 0 && bodyB->GetUserData().pointer != 0) {
			GameObject objectA = *(GameObject*)bodyA->GetUserData().pointer;
			GameObject objectB = *(GameObject*)bodyB->GetUserData().pointer;

			if (objectA.HasComponent<ScriptComponent>()) {
				auto& script = objectA.GetComponent<ScriptComponent>();

				if (script.ScriptActive && script.ScriptInstance != nullptr)
					script.ScriptInstance->OnCollisionStart(objectB);
			}

			if (objectB.HasComponent<ScriptComponent>()) {
				auto& script = objectB.GetComponent<ScriptComponent>();

				if (script.ScriptActive && script.ScriptInstance != nullptr)
					script.ScriptInstance->OnCollisionStart(objectA);
			}
		}
	}

	void ContactListener::EndContact(b2Contact* contact)
	{
		b2Body* bodyA = contact->GetFixtureA()->GetBody();
		b2Body* bodyB = contact->GetFixtureB()->GetBody();

		if (bodyA->GetUserData().pointer != 0 && bodyB->GetUserData().pointer != 0) {
			GameObject objectA = *(GameObject*)bodyA->GetUserData().pointer;
			GameObject objectB = *(GameObject*)bodyB->GetUserData().pointer;

			if (objectA.HasComponent<ScriptComponent>()) {
				auto& script = objectA.GetComponent<ScriptComponent>();

				if (script.ScriptActive && script.ScriptInstance != nullptr)
					script.ScriptInstance->OnCollisionEnd(objectB);
			}
		}
	}
}