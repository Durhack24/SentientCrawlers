#include "App.h"

// Libraries
#include "../ImGui/imgui.h"
#include "../ImGui/imgui_impl_glfw.h"
#include "../ImGui/imgui_impl_opengl3.h"

#include "init.h"

bool App::Initialize()
{
    // Initialize GLFW
    bool glInitSuccess = InitializeGlfw();
    if (!glInitSuccess) return false;

    // Create window with graphics context
    bool windowSuccess;
    std::tie(window, windowSuccess) = CreateWindow(1280, 720, "Sentient Crawlers");
    if (!windowSuccess) return false;

    // Initialize ImGui
    bool imGuiInitSuccess = InitializeImGui(window);
    return imGuiInitSuccess;
}

void App::MainLoop()
{
    while (!glfwWindowShouldClose(window))
    {
        // Poll events
        glfwPollEvents();

        // Start the Dear ImGui frame
        StartFrame();

        // Make next window full-screen
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);

        // Render interface
        interface.Render();

        // Rendering
        UpdateViewport();
        RenderImgui();

        glfwSwapBuffers(window);
    }
}

App::~App()
{
    // Cleanup Imgui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // Cleanup glfw
    glfwDestroyWindow(window);
    glfwTerminate();
}

void App::StartFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void App::UpdateViewport()
{
    glfwGetFramebufferSize(window, &w, &h);
    glViewport(0, 0, w, h);
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void App::RenderImgui()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* ctxBackup = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(ctxBackup);
    }
}