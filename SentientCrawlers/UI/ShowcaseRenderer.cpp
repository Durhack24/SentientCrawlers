#include "ShowcaseRenderer.h"

ShowcaseRenderer::ShowcaseRenderer(Simulator& sim_, Point mapSize_, const Crawler& best_)
	: Renderer(sim_, mapSize_), best(best_) {}

void ShowcaseRenderer::Render(ImDrawList* draw, ImVec2 pos, ImVec2 size)
{

}

void ShowcaseRenderer::Step()
{
	Simulator::StepCrawler(best);
}