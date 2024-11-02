#include "Interface.h"

#include <format>

#include "../ImGui/imgui.h"
#include "../ImGui/imgui_impl_glfw.h"
#include "../ImGui/imgui_impl_opengl3.h"

Interface::Interface()
    : open(true) {}

void Interface::Render()
{
    ImGuiStyle& style = ImGui::GetStyle();
    //style.ItemSpacing = { 0.0f, 0.0f };

    // Demo window
    if (ImGui::Begin("Layout", &open, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize))
    {
        ImGui::SetNextWindowSizeConstraints({ 100, -1 }, { 600, -1 });
        ImGui::BeginChild("child1", { 200, 0 }, ImGuiChildFlags_Border | ImGuiChildFlags_ResizeX);
        ImGui::EndChild();

        ImGui::SameLine();
        
        ImGui::BeginChild("child2", { -30, 0});
        ImVec2 size = ImGui::GetContentRegionAvail();
        ImVec2 pos = ImGui::GetCursorScreenPos();

        ImDrawList* draw = ImGui::GetWindowDrawList();
        draw->AddRectFilled(pos, pos + size, IM_COL32(255, 0, 0, 255), 0);
        ImGui::EndChild();

        ImGui::SameLine();

        ImGui::BeginChild("child3");
        ImGui::EndChild();
    }
    ImGui::End();

    ImGui::ShowDemoWindow();
}