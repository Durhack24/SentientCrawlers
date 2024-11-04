#include "Interface.h"

#include <iostream>
#include <format>
#include <thread>
#include <chrono>
#include <numbers>

#include "../Resources/ResourceManager.h"

#include "../ImGui/imgui_impl_glfw.h"
#include "../ImGui/imgui_impl_opengl3.h"

#include "../Simulator/Map.h"

#include "../UI/colorutils.h"

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

static inline double sigmoid(double x)
{
    return 1.0 / (1.0 + exp(-x));
}

void Interface::Render()
{
    const ImGuiWindowFlags windowFlags =
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar
        | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoResize
        | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoBringToFrontOnFocus;

    // Main layout
    if (ImGui::Begin("Layout", &open, windowFlags))
    {
        // Menu
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("View"))
            {
                ImGui::MenuItem("Network Visualizer", "", &showNetworkVisualizer);
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        // Sidebar
        Sidebar();

        // Canvas
        Canvas();
    }
    ImGui::End();

    // Network visualizer
    if (showNetworkVisualizer)
        NetworkVisualizer();
}

void Interface::Sidebar()
{
    ImGui::SetNextWindowSizeConstraints({ 100, -1 }, { INFINITY, -1 });
    ImGui::BeginChild("sidebar", { 200, 0 }, ImGuiChildFlags_Border | ImGuiChildFlags_ResizeX);

    // === Simulator Configuration ===
    static int numCrawlers = 100;
    ImGui::Text("Configuration");
    ImGui::InputInt("Num Crawlers", &numCrawlers, 0);

    ImGui::SliderInt("X Start Pos", &xStartPos, 0, mapImg->Width());
    ImGui::SliderInt("Y Start Pos", &yStartPos, 0, mapImg->Height());
    ImGui::SliderInt("Starting Angle", &startDir, 0, 360);
    double startDirRad = startDir / 360.0 * std::numbers::pi * 2;

    if (ImGui::Button("Create Simulation"))
    {
        sim = std::make_unique<Simulator>(numCrawlers, Point{ xStartPos, yStartPos }, startDirRad);
        renderer = std::make_unique<TrainingRenderer>(*sim.get(), Point{ mapImg->Width(), mapImg->Height() });
        graphData.points.clear();
        graphData.maxY = -INFINITY;
    }

    // === Run Options ===
    if (!sim)
        ImGui::BeginDisabled();

    ImGui::InputInt("Crawl Duration", &crawlDuration);
    if (ImGui::Button("One Minute") && simState == SimulatorState::Idle)
        simState = SimulatorState::RunningOneMinute;
    ImGui::SameLine();
    if (ImGui::Button("One Generation") && simState == SimulatorState::Idle)
        simState = SimulatorState::RunningOneGen;
    ImGui::SameLine();
    ImGui::PushID(1001);
    if (ImGui::Button(simState == SimulatorState::RunningNonstop ? "Stop Running" : "Run Nonstop"))
    {
        if (simState == SimulatorState::RunningNonstop)
            simState = SimulatorState::Idle;
        else if (simState == SimulatorState::Idle)
            simState = SimulatorState::RunningNonstop;
    }
    ImGui::PopID();

    static bool showBestCrawler = false;
    if (ImGui::Checkbox("Show Best Crawler", &showBestCrawler))
    {
        renderer.reset();
        if (showBestCrawler)
            renderer = std::make_unique<ShowcaseRenderer>(*sim.get(), Point{ mapImg->Width(), mapImg->Height() }, sim->GetCrawlers()[0].second);
        else
            renderer = std::make_unique<TrainingRenderer>(*sim.get(), Point{ mapImg->Width(), mapImg->Height() });
    }

    if (showBestCrawler)
    {
        static int numSteps = 1;
        if (ImGui::Button("Step"))
            ((ShowcaseRenderer*)renderer.get())->Step(numSteps);

        ImGui::SameLine();
        ImGui::InputInt("Num", &numSteps);
    }

    // === Statistics ===
    UpdateStatistics();

    // Statistics text
    ImGui::Text("Generation: %zu", sim ? sim->GetGeneration() : 0);
    ImGui::Text("Max Bars: %d", maxBarsVisited);

    // Graph
    ImGui::BeginChild("graph", { 0, -FLT_MIN }, ImGuiChildFlags_Border);
    ImDrawList* draw = ImGui::GetWindowDrawList();
    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImVec2 size = ImGui::GetContentRegionAvail();

    draw->AddRectFilled(pos, pos + size, IM_COL32(50, 50, 50, 255));

    if (!graphData.points.empty())
        RenderGraph(draw, pos, size);
    ImGui::EndChild();

    if (!sim)
        ImGui::EndDisabled();

    ImGui::EndChild();
}

void Interface::Canvas()
{
    ImGui::SameLine();
    ImGui::BeginChild("canvas");

    // Useful variables
    ImDrawList* draw = ImGui::GetWindowDrawList();
    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImVec2 size = ImGui::GetContentRegionAvail();

    // Render the map and the bars
    RenderBackdrop(draw, pos, size);

    // Render start pos
    double startDirRad = ToRadians(startDir);
    ImVec2 screenStartPos = PointToScreen(pos, size, Point{ xStartPos, yStartPos });
    draw->AddCircleFilled(screenStartPos, 5.0f, IM_COL32(255, 0, 255, 255));
    draw->AddLine(screenStartPos, screenStartPos + ImVec2(cos(startDirRad), -sin(startDirRad)) * 20, IM_COL32(255, 0, 255, 255));

    // Render crawlers
    if (renderer)
        renderer->Render(draw, pos, size);

    ImGui::EndChild();
}

void Interface::NetworkVisualizer()
{
    ImGui::SetNextWindowSize({ 600, 600 }, ImGuiCond_Appearing);
    if (ImGui::Begin("Network Visualizer", &showNetworkVisualizer))
    {
        float nodeRadius = 10.0f;

        ImDrawList* draw = ImGui::GetWindowDrawList();
        ImVec2 pos = ImGui::GetCursorScreenPos();
        ImVec2 size = ImGui::GetContentRegionAvail();

        draw->AddRectFilled(pos, pos + size, IM_COL32(50, 50, 50, 255));

        if (!graphData.points.empty())
        {
            // Get information from brain
            const auto arch = graphData.bestBrain.GetArchitecture();
            const auto& weights = graphData.bestBrain.GetWeights();

            // Determine node points
            std::vector<std::vector<ImVec2>> nodePoints;
            double xSpacing = size.x / (arch.size() + 1);
            for (size_t layer = 0; layer < arch.size(); layer++)
            {
                size_t numNodes = (layer == arch.size() - 1) ? arch[layer] : arch[layer] + 1;

                nodePoints.push_back({});

                double ySpacing = size.y / (numNodes + 1);
                for (size_t nodeIdx = 0; nodeIdx < numNodes; nodeIdx++)
                {
                    float nx = pos.x + xSpacing * (layer + 1);
                    float ny = pos.y + (size.y - ySpacing * (nodeIdx + 1));
                    nodePoints[layer].push_back(ImVec2{ nx, ny });
                }
            }

            // Draw weights
            for (size_t layer = 0; layer < arch.size() - 1; layer++)
            {
                for (size_t leftNode = 0; leftNode < arch[layer] + 1; leftNode++)
                {
                    for (size_t rightNode = 0; rightNode < arch[layer + 1]; rightNode++)
                    {
                        double weight = weights[layer][leftNode * arch[layer + 1] + rightNode];

                        auto [r, g, b] = hsv2rgb(sigmoid(weight) / 3 - 0.15f, 1.0, 1.0);
                        draw->AddLine(nodePoints[layer][leftNode] + ImVec2(nodeRadius, 0),
                            nodePoints[layer + 1][rightNode] - ImVec2(nodeRadius, 0), IM_COL32(r, g, b, 255));
                    }
                }
            }

            // Draw Nodes
            for (const auto& layer : nodePoints)
                for (const ImVec2& nodePos : layer)
                    draw->AddCircle(nodePos, nodeRadius, IM_COL32_WHITE);
        }

        ImGui::End();
    }
}

ImVec2 Interface::PointToScreen(ImVec2 canvasPos, ImVec2 canvasSize, Point p)
{
    float screenX = (p.x / mapImg->Width()) * canvasSize.x + canvasPos.x;
    float screenY = (1.0 - p.y / mapImg->Height()) * canvasSize.y + canvasPos.y;

    return { screenX, screenY };
}

void Interface::RenderGraph(ImDrawList* draw, ImVec2 pos, ImVec2 size)
{
    size_t maxX = graphData.points.back().first;
    for (size_t percentile = 0; percentile <= 10; percentile++)
    {
        for (size_t dataIdx = 0; dataIdx < graphData.points.size() - 1; dataIdx++)
        {
            size_t x0 = graphData.points[dataIdx].first;
            double y0 = graphData.points[dataIdx].second[percentile];

            size_t x1 = graphData.points[dataIdx + 1].first;
            double y1 = graphData.points[dataIdx + 1].second[percentile];

            float screenX0 = (float)x0 / maxX * size.x + pos.x;
            float screenX1 = (float)x1 / maxX * size.x + pos.x;

            float screenY0 = (1 - y0 / graphData.maxY) * size.y + pos.y;
            float screenY1 = (1 - y1 / graphData.maxY) * size.y + pos.y;

            ImU32 col = (percentile == 5) ? IM_COL32(255, 0, 0, 255) : IM_COL32_WHITE;
            float thickness = (percentile == 5) ? 2.0f : 0.5f;
            draw->AddLine({ screenX0, screenY0 }, { screenX1, screenY1 }, col, thickness);
        }
    }
}

void Interface::RenderBackdrop(ImDrawList* draw, ImVec2 pos, ImVec2 size)
{
    // Draw the map
    draw->AddImage((ImTextureID)(intptr_t)mapImg->Id(), pos, pos + size);
    draw->AddRectFilled(pos, pos + size, IM_COL32(0, 0, 0, 50));

    // Draw the bars
    const auto& bars = Map::GetBars();
    for (const auto& bar : bars)
    {
        draw->AddEllipseFilled(PointToScreen(pos, size, bar.pos), 16.0f / mapImg->Width() * size.x,
            16.0f / mapImg->Height() * size.y, IM_COL32(255, 0, 0, 180));
    }
}

void Interface::UpdateStatistics()
{
    if (!sim)
        return;

    auto crawlers = sim->GetCrawlers();
    size_t gen = sim->GetGeneration();

    // Update bars visited
    maxBarsVisited = crawlers[0].second.numVisitedBars;

    // Update brain
    graphData.bestBrain = crawlers[0].second.brain;

    // Update points
    Percentiles percentiles;
    for (size_t percentile = 0; percentile <= 10; percentile++)
        percentiles[percentile] = crawlers[(crawlers.size() - 1) * (10 - percentile) / 10].first;
    graphData.points.push_back({ gen, percentiles });

    // Update maxY
    graphData.maxY = std::max(graphData.maxY, crawlers[0].first);
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
        case SimulatorState::RunningNonstop:
            RunNonstop();
            break;
        }
    }
}

void Interface::RunOneMinute()
{
    sim->Step(1);
}

void Interface::RunOneGen()
{
    sim->Step(crawlDuration);
    sim->NextGeneration();
}

void Interface::RunNonstop()
{
    using namespace std::chrono_literals;

    while (simState == SimulatorState::RunningNonstop)
    {
        sim->Step(crawlDuration);
        sim->NextGeneration();
    }

    sim->UpdateBuf();
}

double Interface::ToRadians(int deg)
{
    return deg / 180.0 * std::numbers::pi;
}