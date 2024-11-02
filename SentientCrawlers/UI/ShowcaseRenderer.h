#pragma once
#include "Renderer.h"

class ShowcaseRenderer : public Renderer
{
public:
	ShowcaseRenderer(Simulator& sim_, Point mapSize_);

	void Render(ImDrawList* draw, ImVec2 pos, ImVec2 size) override;
};