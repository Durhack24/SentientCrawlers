#include "Interface.h"

#include <iostream>
#include <format>

#include "../Resources/ResourceManager.h"

#include "../ImGui/imgui.h"
#include "../ImGui/imgui_impl_glfw.h"
#include "../ImGui/imgui_impl_opengl3.h"

Interface::Interface()
    : open(true), simThread([this]() { this->SimulatorThread(); })
{
    mapImg = std::make_unique<Image>(ResourceManager::GetResourcePath("map.png"));
}

Interface::~Interface()
{
    simState = SimulatorState::Idle;
    open = false;
    simThread.join();
}

void Interface::Render()
{
    ImGuiStyle& style = ImGui::GetStyle();

    if (ImGui::Begin("Layout", &open, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize))
    {
        // === Sidebar ===
        ImGui::SetNextWindowSizeConstraints({ 100, -1 }, { 600, -1 });
        ImGui::BeginChild("sidebar", { 200, 0 }, ImGuiChildFlags_Border | ImGuiChildFlags_ResizeX);

        // Configuration
        static int numCrawlers = 100;
        ImGui::Text("Configuration");
        ImGui::InputInt("Num Crawlers", &numCrawlers);
        if (ImGui::Button("Create Simulation"))
            sim = std::make_unique<Simulator>(numCrawlers, Point{ 600, 973 });

        ImGui::Separator();

        // Options
        if (!sim)
            ImGui::BeginDisabled();

        ImGui::InputInt("Crawl Duration", &crawlDuration);
        if (ImGui::Button("One Generation") && simState == SimulatorState::Idle)
            simState = SimulatorState::RunningOneGen;
        if (ImGui::Button("Run at Max"))
            if (simState == SimulatorState::RunningAtMax)
                simState = SimulatorState::Idle;
            else if (simState == SimulatorState::Idle)
                simState = SimulatorState::RunningAtMax;

        // Graph
        ImGui::BeginChild("graph", { 0, 200 }, ImGuiChildFlags_Border);
        ImGui::EndChild();

        if (!sim)
            ImGui::EndDisabled();

        ImGui::EndChild();

        // === Canvas ===
        ImGui::SameLine();
        ImGui::BeginChild("canvas");
        ImVec2 size = ImGui::GetContentRegionAvail();
        ImVec2 pos = ImGui::GetCursorScreenPos();

        ImDrawList* draw = ImGui::GetWindowDrawList();
        draw->AddImage((ImTextureID)(intptr_t)mapImg->Id(), pos, pos + size);
        ImGui::EndChild();
    }
    ImGui::End();
}

void Interface::SimulatorThread()
{
    while (open)
    {
        switch (simState)
        {
        case SimulatorState::Idle:
            break;
        case SimulatorState::RunningOneGen:
            RunOneGen();
            simState = SimulatorState::Idle;
            break;
        case SimulatorState::RunningAtMax:
            RunAtMax();
            break;
        }
    }
}

void Interface::RunOneGen()
{
    sim->Step(crawlDuration);
    sim->NextGeneration();
}

void Interface::RunAtMax()
{
    static size_t counter = 0;
    while (simState == SimulatorState::RunningAtMax)
    {
        sim->Step(crawlDuration);
        sim->NextGeneration();
        std::cout << std::format("Generation: {}\n", counter++);
    }
}