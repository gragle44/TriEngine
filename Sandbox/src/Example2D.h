#pragma once

#include <TriEngine.h>

class Example2D : public TriEngine::Layer
{
public:
	Example2D();
	virtual ~Example2D() = default;

	virtual void OnAttach() final;
	virtual void OnDetach() final;

	void OnUpdate(float deltaTime) final;
	virtual void OnImGuiRender() final;
	void OnEvent(TriEngine::Event& e) final;
	bool OnWindowResized(TriEngine::WindowResizeEvent& e);
private:
	TriEngine::OrthographicCameraController m_CameraController;
	TriEngine::Reference<TriEngine::FrameBuffer> m_FrameBuffer;

	TriEngine::Reference<TriEngine::Texture2D> m_Texture;
	TriEngine::Reference<TriEngine::Texture2D> m_Texture2;
	TriEngine::Reference<TriEngine::Texture2D> m_Texture3;
	TriEngine::Reference<TriEngine::Texture2D> m_GradientTexture;
	TriEngine::Reference<TriEngine::Texture2D> m_CheckerBoard;
	TriEngine::Reference<TriEngine::TextureAtlas> m_ChestAtlas;
	TriEngine::Reference<TriEngine::SubTexture2D> m_Chest1;

	std::vector<TriEngine::TexturedQuad> m_Quads;
};