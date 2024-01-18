#include <TriEngine.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "imgui.h"

class ExampleLayer : public TriEngine::Layer {
public:
    ExampleLayer()
        : Layer("Example"), m_Camera(1280, 720), m_CameraPos(0.0f, 0.0f, 0.0f), m_TrianglePos(0.0f, 0.0f, 0.0f) {
    }

    void OnAttach() final {
        m_VertexArray.reset(TriEngine::VertexArray::Create());

        float vertices[4 * 7] = {
            -0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
             0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
             0.5f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f,
             -0.5f,  0.5f, 0.0f, 0.6f, 0.85f, 0.2f, 1.0f
        };

        m_VertexBuffer.reset(TriEngine::VertexBuffer::Create(vertices, sizeof(vertices)));

        {
            TriEngine::BufferLayout layout = {
                { "a_Position", TriEngine::ShaderDataType::Float3 },
                { "a_Color", TriEngine::ShaderDataType::Float4 }
            };

            m_VertexBuffer->SetLayout(layout);
        }

        uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };
        m_IndexBuffer.reset(TriEngine::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

        m_VertexArray->AddVertexAndIndexBuffers(m_VertexBuffer, m_IndexBuffer);

        m_Shader.reset(TriEngine::Shader::Create("src/Shaders/basicvert.glsl", "src/Shaders/basicfrag.glsl"));
    }

    void OnUpdate(float deltaTime) final {
        TRI_TRACE(deltaTime * 1000.0f);
        TriEngine::RenderCommand::ClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
        TriEngine::RenderCommand::Clear();

        if (TriEngine::Input::IsKeyPressed(TRI_KEY_RIGHT)) {
            m_Camera.SetRotation(m_Camera.GetRotation() - 25.0f * deltaTime);
        } 
        else if (TriEngine::Input::IsKeyPressed(TRI_KEY_LEFT)) {
            m_Camera.SetRotation(m_Camera.GetRotation() + 25.0f * deltaTime);
        }

        if (TriEngine::Input::IsKeyPressed(TRI_KEY_UP)) {
            m_Camera.SetZoom(m_Camera.GetZoom() - 8.0f * deltaTime);
        }
        else if (TriEngine::Input::IsKeyPressed(TRI_KEY_DOWN)) {
            m_Camera.SetZoom(m_Camera.GetZoom() + 8.0f * deltaTime);
        }

        glm::vec3 direction(0.0f, 0.0f, 0.0f);

        if (TriEngine::Input::IsKeyPressed(TRI_KEY_A))
            direction = { -m_CameraSpeed, 0.0f, 0.0f };
        if (TriEngine::Input::IsKeyPressed(TRI_KEY_D))
            direction = { m_CameraSpeed, 0.0f, 0.0f };
        if (TriEngine::Input::IsKeyPressed(TRI_KEY_W))
            direction = { 0.0f, m_CameraSpeed, 0.0f };
        if (TriEngine::Input::IsKeyPressed(TRI_KEY_S))
            direction = { 0.0f, -m_CameraSpeed, 0.0f };

        m_CameraPos += direction * deltaTime;

        m_Camera.SetPosition(m_CameraPos);
        TriEngine::Renderer::Begin(m_Camera);

        glm::mat4 scale = glm::scale(glm::mat4(1.0f), { 0.1f, 0.1f, 1.0f });

        for (int y = 0; y < 25; y++) {
            for (int x = 0; x < 25; x++) {
                glm::vec3 pos(0.11f * x, 0.11f * y, 0.0f);
                glm::mat4 translation = glm::translate(glm::mat4(1.0f), pos) * scale;
                TriEngine::Renderer::Submit(m_Shader, m_VertexArray, translation);
            }
        }

        TriEngine::Renderer::End();
    }

    void OnImGuiRender() final {

    }

    void OnEvent(TriEngine::Event& e) final {

    }

private:
    const float m_CameraSpeed = 1.15f;

    glm::vec3 m_CameraPos;
    glm::vec3 m_TrianglePos;

    TriEngine::OrthographicCamera m_Camera;
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