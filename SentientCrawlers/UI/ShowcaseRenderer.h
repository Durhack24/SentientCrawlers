#pragma once
#include "Renderer.h"
#include "../Simulator/Crawler.h"

class ShowcaseRenderer : public Renderer
{
public:
	ShowcaseRenderer(Simulator& sim_, Point mapSize_, const Crawler& best_);
	virtual ~ShowcaseRenderer() = default;

	void Render(ImDrawList* draw, ImVec2 pos, ImVec2 size) override;
	void Step(size_t num = 1);

protected:
	Crawler best;
	std::vector<Point> path;
};