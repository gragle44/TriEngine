#include <TriEngine.h>
#include <Core/Base/EntryPoint.h>

#include "EditorApp.h"

class EditorRuntime : public TriEngine::Application {
public:
    EditorRuntime(const ApplicationConfiguration& config) 
        :Application(config)
    {
        PushLayer(new EditorLayer());
    }

    ~EditorRuntime() {

    }
};

TriEngine::Application* TriEngine::CreateApplication(int argc, char** argv) {
    ApplicationConfiguration config{.Name = "TriEditor"};
    return new EditorRuntime(config);
}