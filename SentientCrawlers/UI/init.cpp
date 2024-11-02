#include "init.h"

#include <optional>

#include "../ImGui/imgui.h"
#include "../ImGui/imgui_impl_glfw.h"
#include "../ImGui/imgui_impl_opengl3.h"

#include "err.h"
#include "theme.h"

bool InitializeGlfw()
{
    // Set error callback for GLFW
    glfwSetErrorCallback(GlfwErrorCallback);

    // Call glfwInit and check result
    if (!glfwInit())
        return false;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    return true;
}

std::pair<GLFWwindow*, bool> CreateWindow(uint32_t w, uint32_t h, const std::string& title)
{
    // Create window
    GLFWwindow* window = glfwCreateWindow(w, h, title.c_str(), nullptr, nullptr);
    if (window == nullptr) return { nullptr, false };

    // Setup window
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    return { window, true };
}

bool InitializeImGui(GLFWwindow* window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    SetTheme();

    // Setup Platform/Renderer backends
    const char* glsl_version = "#version 410";
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Load Fonts
    io.FontDefault = io.Fonts->AddFontFromFileTTF("C:\\Users\\matty\\source\\repos\\ImguiTesting\\ImguiTesting\\UI\\Montserrat-Regular.ttf", 18.0f);

    return true;
}