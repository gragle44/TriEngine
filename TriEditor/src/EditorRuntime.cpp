#include <TriEngine.h>
#include <Core/Base/EntryPoint.h>

#include "EditorApp.h"

class EditorRuntime : public TriEngine::Application {
public:
    EditorRuntime() 
        :Application("TriEditor")
    {
        PushLayer(new EditorLayer());
    }

    ~EditorRuntime() {

    }
};

TriEngine::Application* TriEngine::CreateApplication() {
    return new EditorRuntime();
}