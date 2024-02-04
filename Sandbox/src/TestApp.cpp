#include <TriEngine.h>
#include <Core/Base/EntryPoint.h>

#include "Example2D.h"

class Sandbox : public TriEngine::Application {
public:
    Sandbox() {
        PushLayer(new Example2D());
    }

    ~Sandbox() {

    }
};

TriEngine::Application* TriEngine::CreateApplication() {
    return new Sandbox();
}