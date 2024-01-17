#include <TriEngine.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "imgui.h"

class ExampleLayer : public TriEngine::Layer {
public:
    ExampleLayer()
        : Layer("Example"), m_ColorModifier(0.3f, 0.0f, 0.0f, 1.0f), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f) {
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

        m_Shader.reset(TriEngine::Shader::Create("src/Shaders/basicvert.glsl", "src/Shaders/basicfrag.glsl"));
    }

    void OnUpdate() final {
        TriEngine::RenderCommand::ClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
        TriEngine::RenderCommand::Clear();

        if (TriEngine::Input::IsKeyPressed(TRI_KEY_RIGHT)) {
            m_Camera.SetRotation(m_Camera.GetRotation() + 2.0f);
        } 
        else if (TriEngine::Input::IsKeyPressed(TRI_KEY_LEFT)) {
            m_Camera.SetRotation(m_Camera.GetRotation() - 2.0f);
        }

        m_Camera.SetPosition({ 0.0f, 0.0f, 0.0f });
        TriEngine::Renderer::Begin(m_Camera);

        TriEngine::Renderer::Submit(m_Shader, m_VertexArray);

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
    TriEngine::OrthographicCamera m_Camera;
    glm::vec4 m_ColorModifier;
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