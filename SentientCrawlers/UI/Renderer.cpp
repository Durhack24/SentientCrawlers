#include "Renderer.h"

Renderer::Renderer(Simulator& sim_, Point mapSize_)
	: sim(sim_), mapSize(mapSize_) { }

ImVec2 Renderer::PointToScreen(ImVec2 canvasPos, ImVec2 canvasSize, Point p) const
{
	float screenX = (p.x / mapSize.x) * canvasSize.x + canvasPos.x;
	float screenY = (1.0 - p.y / mapSize.y) * canvasSize.y + canvasPos.y;

	return { screenX, screenY };
}