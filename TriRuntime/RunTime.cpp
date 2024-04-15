#include <TriEngine.h>
#include <Core/Base/EntryPoint.h>

#include "App.h"

class Sandbox : public TriEngine::Application {
public:
    Sandbox()
        :Application("App")
    {
        PushLayer(new App());
    }

    ~Sandbox() {

    }
};

TriEngine::Application* TriEngine::CreateApplication() {
    return new Sandbox();
}