#include <iostream>

#include "UI/App.h"

int main()
{
    // Initialize
    App app;
    bool initSuccess = app.Initialize();
    if (!initSuccess) return 1;

    char* versionStr = (char*)glGetString(GL_VERSION);
    std::cout << "OpenGL Version: " << versionStr;

    // Main loop
    app.MainLoop();
}