#include <TriEngine.h>

class Sandbox : public TriEngine::Application {
public:
    Sandbox() {

    }

    ~Sandbox() {

    }
};

TriEngine::Application* TriEngine::CreateApplication() {
    return new Sandbox();
}