#include "TrainingRenderer.h"

#include "colorutils.h"

TrainingRenderer::TrainingRenderer(Simulator& sim_, Point mapSize_)
    : Renderer(sim_, mapSize_) {}

void TrainingRenderer::Render(ImDrawList* draw, ImVec2 pos, ImVec2 size)
{
    auto crawlers = sim.GetCrawlers();
    double costMin = INFINITY, costMax = -INFINITY;
    for (const auto& [cost, crawler] : crawlers)
    {
        costMin = std::min(costMin, cost);
        costMax = std::max(costMax, cost);
    }

    for (const auto& [cost, crawler] : crawlers)
    {
        double t = (cost + costMin) / (costMax - costMin);
        auto [r, g, b] = hsv2rgb(85 * t, 255, 255);
        draw->AddCircleFilled(PointToScreen(pos, size, crawler.pos), 5.0f, IM_COL32(r, g, b, 255));
    }
}