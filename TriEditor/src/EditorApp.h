#pragma once

#include <TriEngine.h>

using namespace TriEngine;

class EditorLayer : public TriEngine::Layer
{
public:
	EditorLayer();
	virtual ~EditorLayer() = default;

	virtual void OnAttach() final;
	virtual void OnDetach() final;

	void OnUpdate(float deltaTime) final;
	virtual void OnImGuiRender() final;
	void OnEvent(Event& e) final;
private:

};