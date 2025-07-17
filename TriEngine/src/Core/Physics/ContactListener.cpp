#include "tripch.h"
#include "ContactListener.h"

#include "Core/GameObjects/GameObject.h"
#include "Core/GameObjects/Components.h"

#include "Core/Scripting/ScriptEngine.h"

#include "box2d/b2_contact.h"

namespace TriEngine {
	void TriEngine::ContactListener::BeginContact(b2Contact* contact)
	{
		b2Body* bodyA = contact->GetFixtureA()->GetBody();
		b2Body* bodyB = contact->GetFixtureB()->GetBody();

		if (bodyA->GetUserData().pointer && bodyB->GetUserData().pointer) {
			GameObject objectA = *(GameObject*)bodyA->GetUserData().pointer;
			GameObject objectB = *(GameObject*)bodyB->GetUserData().pointer;

			ScriptEngine& scriptEngine = ScriptEngine::Get();
			
			if (objectA.HasComponent<ScriptComponent>()) {
				auto& script = objectA.GetComponent<ScriptComponent>();

				if (script.Active)
					scriptEngine.OnCollisionStart(script.Build, objectB);
			}

			if (objectB.HasComponent<ScriptComponent>()) {
				auto& script = objectB.GetComponent<ScriptComponent>();

				if (script.Active)
					scriptEngine.OnCollisionStart(script.Build, objectA);
			}

			if (objectA.HasComponent<NativeScriptComponent>()) {
				auto& script = objectA.GetComponent<NativeScriptComponent>();

				if (script.ScriptActive && script.ScriptInstance != nullptr)
					script.ScriptInstance->OnCollisionStart(objectB);
			}

			if (objectB.HasComponent<NativeScriptComponent>()) {
				auto& script = objectB.GetComponent<NativeScriptComponent>();

				if (script.ScriptActive && script.ScriptInstance != nullptr)
					script.ScriptInstance->OnCollisionStart(objectA);
			}
		}
	}

	void ContactListener::EndContact(b2Contact* contact)
	{
		b2Body* bodyA = contact->GetFixtureA()->GetBody();
		b2Body* bodyB = contact->GetFixtureB()->GetBody();

		if (bodyA->GetUserData().pointer && bodyB->GetUserData().pointer) {
			GameObject objectA = *(GameObject*)bodyA->GetUserData().pointer;
			GameObject objectB = *(GameObject*)bodyB->GetUserData().pointer;

			ScriptEngine& scriptEngine = ScriptEngine::Get();

			if (objectA.HasComponent<ScriptComponent>()) {
				auto& script = objectA.GetComponent<ScriptComponent>();

				if (script.Active)
					scriptEngine.OnCollisionStop(script.Build, objectB);
			}

			if (objectB.HasComponent<ScriptComponent>()) {
				auto& script = objectB.GetComponent<ScriptComponent>();

				if (script.Active)
					scriptEngine.OnCollisionStop(script.Build, objectA);
			}

			if (objectA.HasComponent<NativeScriptComponent>()) {
				auto& script = objectA.GetComponent<NativeScriptComponent>();

				if (script.ScriptActive && script.ScriptInstance != nullptr)
					script.ScriptInstance->OnCollisionEnd(objectB);
			}
		}
	}
}