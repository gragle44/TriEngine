#include <TriEngine.h>
#include <Core/Base/EntryPoint.h>

#include "App.h"

// TODO: Define this in build system instead of here
#define GAME_DATA_LOCATION "game.pck"

using namespace TriEngine;

class Runtime : public TriEngine::Application {
public:
    Runtime(const ApplicationConfiguration& config)
        :Application(config)
    {
        PushLayer(new App());

        GetWindow().SetVSync(ProjectManager::GetCurrent()->GetProjectData().WindowSettings.VSync);
    }

    ~Runtime() {

    }
};

TriEngine::Application* TriEngine::CreateApplication(int argc, char** argv) {

    std::filesystem::path path;
    if (argc == 1) {
        path = std::filesystem::current_path() / GAME_DATA_LOCATION;
    }
    else {
        path = argv[1];
    }

    if (std::filesystem::exists(path)) {
        auto extension = path.extension();
        if (extension == ".tri") {
            ProjectManager::Load(path, false);
        }
        else if (extension == ".pck") {
            ProjectManager::Load(path, true);
        }
        else {
            TRI_CORE_ERROR("Invalid project extension");
            return nullptr;
        }
    }
    else {
        TRI_CORE_ERROR("Could not find project path '{}'", path);
        return nullptr;
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