#include <iostream>

#include "UI/App.h"
#include "Resources/ResourceManager.h"
#include "Simulator/Map.h"

int main(int, char** argv)
{
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
        std::cerr << "Failed to initialize app!\n";
        std::terminate();
    }

    // Log OpenGL version
    char* versionStr = (char*)glGetString(GL_VERSION);
    std::cout << "OpenGL Version: " << versionStr;

    // Main loop
    app.MainLoop();
}