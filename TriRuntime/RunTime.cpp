#include <TriEngine.h>
#include <Core/Base/EntryPoint.h>

#include "App.h"

using namespace TriEngine;

class Runtime : public TriEngine::Application {
public:
    Runtime(const ApplicationConfiguration& config)
        :Application(config)
    {
        PushLayer(new App());
    }

    ~Runtime() {

    }
};

TriEngine::Application* TriEngine::CreateApplication(int argc, char** argv) {

    if (argc == 0) {
        TRI_CORE_ERROR("Please provide a project path to load");
        return nullptr;
    }

    const char* filepath = argv[1];
    if (std::filesystem::exists(filepath)) {
        const char* extension = &filepath[strlen(filepath) - 4];
        if (strcmp(extension, ".tri") != 0) {
            TRI_CORE_ERROR("Invalid project extension");
            return nullptr;
        }
        ProjectManager::Load(filepath);
    }

    auto& projectData = ProjectManager::GetCurrent()->GetProjectData();

    ApplicationConfiguration appConfig{
        .Name = projectData.Name, 
        .WindowWidth = projectData.WindowSettings.Width,
        .WindowHeight = projectData.WindowSettings.Height,
        .Resizable = projectData.WindowSettings.Resizable
    };

    return new Runtime(appConfig);
}