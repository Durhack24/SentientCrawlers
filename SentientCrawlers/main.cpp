#include "UI/App.h"

int main()
{
    // Initialize
    App app;
    bool initSuccess = app.Initialize();
    if (!initSuccess) return 1;

    // Main loop
    app.MainLoop();
}