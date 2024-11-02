#pragma once
#include "Renderer.h"

class TrainingRenderer : public Renderer
{
public:
	TrainingRenderer(Simulator& sim_, Point mapSize_);

	void Render(ImDrawList* draw, ImVec2 pos, ImVec2 size) override;
};