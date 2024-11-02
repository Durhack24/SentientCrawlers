#include <iostream>

#include "UI/App.h"
#include "Resources/ResourceManager.h"
#include "Simulator/Map.h"

int main(int argc, char** argv)
{
#if 1
    // Find resources
    ResourceManager::AddSearchPath(argv[0]);
    ResourceManager::FindResources();

    // Load map
    Map::Load();

    // Initialize
    App app;
    bool initSuccess = app.Initialize();
    if (!initSuccess)
    {
        std::cerr << "app.Initialize() Failed!\n";
        std::terminate();
    }

    char* versionStr = (char*)glGetString(GL_VERSION);
    std::cout << "OpenGL Version: " << versionStr;

    // Main loop
    app.MainLoop();
#else

#endif
}