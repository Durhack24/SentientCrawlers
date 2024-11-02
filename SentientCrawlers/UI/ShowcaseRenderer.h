#pragma once
#include "Renderer.h"
#include "../Simulator/Crawler.h"

class ShowcaseRenderer : public Renderer
{
public:
	ShowcaseRenderer(Simulator& sim_, Point mapSize_, const Crawler& best_);

	void Render(ImDrawList* draw, ImVec2 pos, ImVec2 size) override;
	void Step();

protected:
	Crawler best;
};