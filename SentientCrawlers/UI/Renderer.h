#pragma once
#include "../ImGui/imgui.h"
#include "../Simulator/Simulator.h"

class Renderer
{
public:
	Renderer(Simulator& sim_, Point mapSize_);

	virtual void Render(ImDrawList* draw, ImVec2 pos, ImVec2 size) = 0;

protected:
	Simulator& sim;
	Point mapSize;

	ImVec2 PointToScreen(ImVec2 canvasPos, ImVec2 canvasSize, Point p) const;
};