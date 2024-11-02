#include "Interface.h"

#include <iostream>
#include <format>

#include "../Resources/ResourceManager.h"

#include "../ImGui/imgui_impl_glfw.h"
#include "../ImGui/imgui_impl_opengl3.h"

#include "../Simulator/Map.h"

Interface::Interface()
    : open(true), simThread([this]() { this->SimulatorThread(); })
{
    mapImg = std::make_unique<Image>(ResourceManager::GetResourcePath("map.png"));
}

Interface::~Interface()
{
    simState = SimulatorState::Idle;
    runSimThread = false;
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
        {
            sim = std::make_unique<Simulator>(numCrawlers, Point{ 620, 1030 - 950 });
            renderer = std::make_unique<TrainingRenderer>(*sim.get(), Point{ mapImg->Width(), mapImg->Height() });
        }

        ImGui::Separator();

        // Options
        if (!sim)
            ImGui::BeginDisabled();

        ImGui::InputInt("Crawl Duration", &crawlDuration);
        if (ImGui::Button("One Minute") && simState == SimulatorState::Idle)
            simState = SimulatorState::RunningOneMinute;
        if (ImGui::Button("One Generation") && simState == SimulatorState::Idle)
            simState = SimulatorState::RunningOneGen;
        if (ImGui::Button("Run at Max"))
            if (simState == SimulatorState::RunningAtMax)
                simState = SimulatorState::Idle;
            else if (simState == SimulatorState::Idle)
                simState = SimulatorState::RunningAtMax;

        static bool showBestCrawler = false;
        if (ImGui::Checkbox("Show Best Crawler", &showBestCrawler))
        {
            renderer.reset();
            if (showBestCrawler)
                renderer = std::make_unique<ShowcaseRenderer>(*sim.get(), Point{ mapImg->Width(), mapImg->Height() });
            else
                renderer = std::make_unique<TrainingRenderer>(*sim.get(), Point{ mapImg->Width(), mapImg->Height() });
        }

        ImGui::Text("Max Bars: %d", maxBarsVisited);

        // Graph
        ImGui::BeginChild("graph", { 0, 200 }, ImGuiChildFlags_Border);
        ImGui::EndChild();

        if (!sim)
            ImGui::EndDisabled();

        ImGui::EndChild();

        // === Canvas ===
        ImGui::SameLine();
        ImGui::BeginChild("canvas");

        // Useful variables
        ImDrawList* draw = ImGui::GetWindowDrawList();
        ImVec2 pos = ImGui::GetCursorScreenPos();
        ImVec2 size = ImGui::GetContentRegionAvail();
        
        // Render the map, the river, and the bars
        RenderBackdrop(draw, pos, size);

        // Render crawlers
        if (renderer)
            renderer->Render(draw, pos, size);

        ImGui::EndChild();
    }
    ImGui::End();
}

ImVec2 Interface::PointToScreen(ImVec2 canvasPos, ImVec2 canvasSize, Point p)
{
    float screenX = (p.x / mapImg->Width()) * canvasSize.x + canvasPos.x;
    float screenY = (1.0 - p.y / mapImg->Height()) * canvasSize.y + canvasPos.y;

    return { screenX, screenY };
}

void Interface::RenderBackdrop(ImDrawList* draw, ImVec2 pos, ImVec2 size)
{
    // Draw the map
    draw->AddImage((ImTextureID)(intptr_t)mapImg->Id(), pos, pos + size);

    // Draw the river
    const auto& river = Map::GetRiver();
    for (size_t i = 0; i < river.size() - 1; i++)
    {
        Point p0 = river[i];
        Point p1 = river[i + 1];
        draw->AddLine(PointToScreen(pos, size, p0), PointToScreen(pos, size, p1), IM_COL32(0, 255, 255, 255), 10.0f);
    }

    // Draw the bars
    const auto& bars = Map::GetBars();
    for (const auto& bar : bars)
    {
        draw->AddEllipseFilled(PointToScreen(pos, size, bar.pos), 16.0f / mapImg->Width() * size.x,
            16.0f / mapImg->Height() * size.y, IM_COL32(255, 0, 0, 255));
    }
}

void Interface::SimulatorThread()
{
    while (runSimThread)
    {
        switch (simState)
        {
        case SimulatorState::Idle:
            break;
        case SimulatorState::RunningOneMinute:
            RunOneMinute();
            simState = SimulatorState::Idle;
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

void Interface::RunOneMinute()
{
    std::cout << "1 minute!\n";
    sim->Step(1);
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