#include "TrainingRenderer.h"

TrainingRenderer::TrainingRenderer(Simulator& sim_, Point mapSize_)
    : Renderer(sim_, mapSize_) {}

void TrainingRenderer::Render(ImDrawList* draw, ImVec2 pos, ImVec2 size)
{
    auto crawlers = sim.GetCrawlers();
    for (const auto& crawler : crawlers)
    {
        float col = ((crawler.numBeatings) * 255.0f);
        draw->AddCircleFilled(PointToScreen(pos, size, crawler.pos), 5.0f, IM_COL32(col, col, col, 255));
    }
}