#include <TriEngine.h>

class ExampleLayer : public TriEngine::Layer {
public:
    ExampleLayer()
        : Layer("Example")
    {
    }

    void OnUpdate() override {
        if (TriEngine::Input::IsMouseButtonPressed(TRI_MOUSE_BUTTON_1))
            TRI_INFO("Mouse 1 clicked!");
    }

    void OnEvent(TriEngine::Event& event) override {
        //TRI_TRACE("{0}", event);
    }
};

class Sandbox : public TriEngine::Application {
public:
    Sandbox() {
        PushLayer(new ExampleLayer());
        PushOverlay(new TriEngine::ImGuiLayer());
    }

    ~Sandbox() {

    }
};

TriEngine::Application* TriEngine::CreateApplication() {
    return new Sandbox();
}