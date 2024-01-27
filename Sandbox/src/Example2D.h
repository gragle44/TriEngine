#pragma once

#include "TriEngine.h"

class Example2D : public TriEngine::Layer
{
public:
	Example2D();
	virtual ~Example2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(float deltaTime) override;
	virtual void OnImGuiRender() override;
	void OnEvent(TriEngine::Event& e) override;
private:
	TriEngine::OrthographicCameraController m_CameraController;
	TriEngine::Reference<TriEngine::Texture2D> m_Texture;
};