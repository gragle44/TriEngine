#pragma once

#include "Core/Base/Core.h"
#include "Core/Base/AssetLibrary.h"
#include "Core/GameObjects/GameObject.h"
#include "Core/GameObjects/Scene.h"
#include "../EditorData.h"

namespace TriEngine {
	class SceneModule {
	public:
		SceneModule(EditorData* data);

		void OnImGuiRender();
	private:
		template<typename T>
		void RenderComponentSelection(std::string_view name, GameObject object, bool* stayOpen);

		void DuplicateObject(GameObject object);

		void DrawNode(GameObject& object);
		void DrawNode(GameObject&& object) { DrawNode(object); }
		void DrawComponents(GameObject& object);

		template<typename T, typename Function> 
		void DrawComponent(const std::string& name, GameObject object, Function function);

		EditorData* m_Data;

		Reference<Scene> m_Scene;

		//Used to render the background for the Sprite2D node
		Reference<Texture2D> m_SpriteBackground;
	};
}