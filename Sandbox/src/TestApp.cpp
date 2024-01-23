#include <TriEngine.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui.h"

class ExampleLayer : public TriEngine::Layer {
public:
    ExampleLayer()
        : Layer("Example"), m_CameraController(TriEngine::OrthographicCamera(1280, 720)), m_CameraPos(0.0f), m_TrianglePos(0.0f), m_ColorModifier(0.0f, 0.5f, 0.95f) {
    }

    void OnAttach() final {
        m_VertexArray.reset(TriEngine::VertexArray::Create());

        float vertices[] = {
           -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
            0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
           -0.5f,  0.5f, 0.0f, 0.0f, 1.0f
        };

        m_VertexBuffer.reset(TriEngine::VertexBuffer::Create(vertices, sizeof(vertices)));

        {
            TriEngine::BufferLayout layout = {
                { "a_Position", TriEngine::ShaderDataType::Float3 },
                { "a_TexCoord", TriEngine::ShaderDataType::Float2 }
            };

            m_VertexBuffer->SetLayout(layout);
        }

        uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };

        m_IndexBuffer.reset(TriEngine::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

        m_VertexArray->AddVertexAndIndexBuffers(m_VertexBuffer, m_IndexBuffer);

        m_Shader.reset(TriEngine::Shader::Create("TextureShader", "src/Shaders/basicvert.glsl", "src/Shaders/basicfrag.glsl"));
        m_ShaderLib.Push(m_Shader->GetName(), m_Shader);

        m_Texture.reset(TriEngine::Texture2D::Create("assets/test2.png"));

        m_Shader->Bind();
        m_Shader->SetInt("u_Texture", 0);
    }

    void OnUpdate(float deltaTime) final {
        TriEngine::RenderCommand::ClearColor({ 0.12f, 0.12f, 0.12f, 1.0f });
        TriEngine::RenderCommand::Clear();

        m_CameraController.OnUpdate(deltaTime);

        TriEngine::Renderer::Begin(m_CameraController.GetCamera());

        glm::mat4 scale = glm::scale(glm::mat4(1.0f), { 0.1f, 0.1f, 1.0f });

        for (int y = 0; y < 25; y++) {
            for (int x = 0; x < 25; x++) {
                glm::vec3 pos(0.11f * x, 0.11f * y, 0.0f);
                glm::mat4 translation = glm::translate(glm::mat4(1.0f), pos) * scale;
                m_Texture->Bind(0);
                TriEngine::Renderer::Submit(m_ShaderLib.Get("TextureShader"), m_VertexArray, translation);
            }
        }

        TriEngine::Renderer::End();
    }

    void OnImGuiRender() final {
        ImGui::Begin("Settings");

        ImGui::End();
    }

    void OnEvent(TriEngine::Event& e) final {
        m_CameraController.OnEvent(e);
        //TriEngine::EventDispatcher dispatcher(e);
    }

private:
    const float m_CameraSpeed = 1.15f;

    glm::vec3 m_CameraPos;
    glm::vec3 m_TrianglePos;
    glm::vec3 m_ColorModifier;

    TriEngine::OrthographicCameraController m_CameraController;
    TriEngine::AssetLibrary<TriEngine::Shader> m_ShaderLib;
    TriEngine::Reference<TriEngine::Shader> m_Shader;
    TriEngine::Reference<TriEngine::Texture2D> m_Texture;
    TriEngine::Reference<TriEngine::VertexArray> m_VertexArray;
    TriEngine::Reference<TriEngine::VertexBuffer> m_VertexBuffer;
    TriEngine::Reference<TriEngine::IndexBuffer> m_IndexBuffer;
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