#include "Simulator.h"

#include <cmath>

#include "Map.h"

Simulator::Simulator(size_t numCrawlers)
{

}

static double Distance(double x, double y, const Point& p)
{
	double dx = x - p.x;
	double dy = y - p.y;
	return std::hypot(dx, dy);
}

static double Distance(double x, double y, const Point& p0, const Point& p1)
{
	double w = p1.x - p0.x;
	double h = p1.y - p0.y;
	double dx = x - p0.x;
	double dy = y - p0.y;

	double t = (dx * w + dy * h) / (w * w + h * h);

	if (t <= 0)
		return Distance(x, y, p0);
	if (t >= 1)
		return Distance(x, y, p1);

	double nearX = p0.x + w * t;
	double nearY = p0.y + h * t;
	return std::hypot(x - nearX, y - nearY);
}

void Simulator::Step(size_t num)
{
	for (size_t stepIdx = 0; stepIdx < num; stepIdx++)
	{
		for (Crawler& crawler : crawlers)
		{
			// Determine stimuli


			// Step the crawler
			//crawler.Step();
		}
	}
}

void Simulator::NextGeneration()
{

}

double Simulator::ClosestBar(const Crawler& crawler)
{
	return 0.0;
}

double Simulator::ClosestBarDir(const Crawler& crawler)
{
	return 0.0;
}