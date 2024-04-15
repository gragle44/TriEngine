#pragma once

#include "Core/Base/Core.h"
#include "Core/GameObjects/GameObject.h"
#include "Core/GameObjects/Scene.h"

namespace TriEngine {
	class SceneModule {
	public:
		SceneModule() = default;
		SceneModule(const Reference<Scene>& scene);

		void SetScene(const Reference<Scene>& scene);

		void OnImGuiRender();
	private:
		template<typename T>
		void RenderComponentSelection(std::string_view name, GameObject object, bool* stayOpen);

		void DuplicateObject(GameObject object);

		void DrawNode(GameObject& object);
		void DrawComponents(GameObject& object);

		template<typename T, typename Function> 
		void DrawComponent(const std::string& name, GameObject object, Function function);

		Reference<Scene> m_Scene;
		GameObject m_SelectedItem;
		GameObject m_RightSelectedItem;

		//Used to render the background for the Sprite2D node
		Reference<Texture2D> m_SpriteBackground;
	};
}