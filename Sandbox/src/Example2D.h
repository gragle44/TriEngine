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
	TriEngine::Reference<TriEngine::Texture2D> m_Texture2;
	TriEngine::Reference<TriEngine::Texture2D> m_Texture3;
	TriEngine::Reference<TriEngine::Texture2D> m_GradientTexture;
	TriEngine::Reference<TriEngine::Texture2D> m_CheckerBoard;
	TriEngine::Reference<TriEngine::TextureAtlas> m_ChestAtlas;
	TriEngine::Reference<TriEngine::SubTexture2D> m_Chest1;

	std::vector<TriEngine::TexturedQuad> m_Quads;
};