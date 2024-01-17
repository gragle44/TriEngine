#include <TriEngine.h>

#include "imgui.h"

class ExampleLayer : public TriEngine::Layer {
public:
    ExampleLayer()
        : Layer("Example") {
    }

    void OnAttach() final {
        m_VertexArray.reset(TriEngine::VertexArray::Create());

        float vertices[3 * 7] = {
            -0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
             0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
             0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
        };

        m_VertexBuffer.reset(TriEngine::VertexBuffer::Create(vertices, sizeof(vertices)));

        {
            TriEngine::BufferLayout layout = {
                { "a_Position", TriEngine::ShaderDataType::Float3 },
                { "a_Color", TriEngine::ShaderDataType::Float4 }
            };

            m_VertexBuffer->SetLayout(layout);
        }

        uint32_t indices[3] = { 0, 1, 2 };
        m_IndexBuffer.reset(TriEngine::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

        m_VertexArray->AddVertexAndIndexBuffers(m_VertexBuffer, m_IndexBuffer);

        m_Shader.reset(new TriEngine::Shader("src/Shaders/basicvert.glsl", "src/Shaders/basicfrag.glsl"));
    }

    void OnUpdate() final {
        TriEngine::RenderCommand::ClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
        TriEngine::RenderCommand::Clear();

        TriEngine::Renderer::Begin();

        m_Shader->Bind();
        TriEngine::Renderer::Submit(m_VertexArray);

        TriEngine::Renderer::End();
    }

    void OnImGuiRender() final {
        ImGui::Begin("HELLO");
        ImGui::Text("i am very gui");
        ImGui::End();
    }

    void OnEvent(TriEngine::Event& e) final {

    }
private:
    std::shared_ptr<TriEngine::Shader> m_Shader;
    std::shared_ptr<TriEngine::VertexArray> m_VertexArray;
    std::shared_ptr<TriEngine::VertexBuffer> m_VertexBuffer;
    std::shared_ptr<TriEngine::IndexBuffer> m_IndexBuffer;
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