#include <TriEngine.h>

#include "imgui.h"

class ExampleLayer : public TriEngine::Layer {
public:
    ExampleLayer()
        : Layer("Example") {
    }

    void OnUpdate() override {

    }

    void OnImGuiRender() override {
        ImGui::Begin("HELLO");
        ImGui::Text("i am very gui");
        ImGui::End();
    }

    void OnEvent(TriEngine::Event& e) override {

    }
};

class Sandbox : public TriEngine::Application {
public:
    Sandbox() {
        PushLayer(new ExampleLayer());
    }

    ~Sandbox() {

    }
};

TriEngine::Application* TriEngine::CreateApplication() {
    return new Sandbox();
}