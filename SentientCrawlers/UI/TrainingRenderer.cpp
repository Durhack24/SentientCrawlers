#include "TrainingRenderer.h"

#include "colorutils.h"

TrainingRenderer::TrainingRenderer(Simulator& sim_, Point mapSize_)
    : Renderer(sim_, mapSize_) {}

void TrainingRenderer::Render(ImDrawList* draw, ImVec2 pos, ImVec2 size)
{
    auto crawlers = sim.GetCrawlers();
    double costMax = -INFINITY;
    for (const auto& [cost, crawler] : crawlers)
        costMax = std::max(costMax, cost);

    for (const auto& [cost, crawler] : crawlers)
    {
        double t = (cost == 0) ? 0 : cost / costMax;
        auto [r, g, b] = hsv2rgb(t / 3.0f, 1.0f, 1.0f);
        if (cost >= 0)
            draw->AddCircleFilled(PointToScreen(pos, size, crawler.pos), 5.0f, IM_COL32(r, g, b, 255));
    }
}