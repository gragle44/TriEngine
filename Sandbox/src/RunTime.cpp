#include <TriEngine.h>
#include <Core/Base/EntryPoint.h>

#include "App.h"

class Runtime : public TriEngine::Application {
public:
    Runtime()
        :Application("App")
    {
        PushLayer(new App());
    }

    ~Runtime() {

    }
};

TriEngine::Application* TriEngine::CreateApplication() {
    return new Runtime();
}