#include <iostream>

#include "UI/App.h"
#include "Resources/ResourceManager.h"

int main(int argc, char** argv)
{
    // Find resources
    ResourceManager::AddSearchPath(argv[0]);
    ResourceManager::FindResources();

    // Initialize
    App app;
    bool initSuccess = app.Initialize();
    if (!initSuccess) return 1;

    char* versionStr = (char*)glGetString(GL_VERSION);
    std::cout << "OpenGL Version: " << versionStr;

    // Main loop
    app.MainLoop();
}