#include "ShowcaseRenderer.h"

#include <iostream>

ShowcaseRenderer::ShowcaseRenderer(Simulator& sim_, Point mapSize_, const Crawler& best_)
	: Renderer(sim_, mapSize_), best(best_)
{
	path.push_back(best.pos);
}

void ShowcaseRenderer::Render(ImDrawList* draw, ImVec2 pos, ImVec2 size)
{
	// Draw path
	for (size_t i = 0; i < path.size() - 1; i++)
	{
		ImVec2 p0 = PointToScreen(pos, size, path[i]);
		ImVec2 p1 = PointToScreen(pos, size, path[i + 1]);

		draw->AddLine(p0, p1, IM_COL32(0, 0, 255, 255), 3.0f);
	}

	// Draw crawler
    draw->AddCircleFilled(PointToScreen(pos, size, best.pos), 5.0f, IM_COL32(0, 0, 200, 255));
}

void ShowcaseRenderer::Step(size_t num)
{
	for (size_t i = 0; i < num; i++)
	{
		Simulator::StepCrawler(best);
        std::cout << best.numBeatings << '\n';
		path.push_back(best.pos);
	}
}