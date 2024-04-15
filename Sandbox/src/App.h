#pragma once

#include <TriEngine.h>

class App : public TriEngine::Layer
{
public:
	App();
	virtual ~App() = default;

	virtual void OnAttach() final;
	virtual void OnDetach() final;

	void OnUpdate(float deltaTime) final;
	virtual void OnImGuiRender() final;
	void OnEvent(TriEngine::Event& e) final;
	bool OnWindowResized(TriEngine::WindowResizeEvent& e);
private:

};