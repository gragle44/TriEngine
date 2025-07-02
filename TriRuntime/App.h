#pragma once

#include "TriEngine.h"

using namespace TriEngine;

class App : public Layer
{
public:
	App();
	virtual ~App() = default;

	virtual void OnAttach() final;
	virtual void OnDetach() final;

	void OnUpdate(float deltaTime) final;
	void OnEvent(Event& e) final;
	bool OnWindowResized(WindowResizeEvent& e);
private:
	std::unique_ptr<GameRenderer> m_Renderer;
	Reference<Scene> m_ActiveScene;
};