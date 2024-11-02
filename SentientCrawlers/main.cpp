#include <iostream>

#include "UI/App.h"
#include "Resources/ResourceManager.h"

#include "Simulator/Brain.h"

int main(int argc, char** argv)
{
#if 0
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
#else
    Brain b = Brain::Random();

    std::vector<double> stimuli{ 1, 2, 3, 4, 5, 6 };
    auto res = b.Think(stimuli);
#endif
}