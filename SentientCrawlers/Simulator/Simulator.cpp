#include "Simulator.h"

#include <cmath>
#include <numbers>

#include "Map.h"

Simulator::Simulator(size_t numCrawlers, const Point& startPos)
{
    crawlers.reserve(numCrawlers);
    for (size_t i = 0; i < numCrawlers; ++i)
        crawlers.emplace_back(startPos.x, startPos.y, 0.0);
}

static inline double NormalizeAngle(double angle)
{
    constexpr double twoPi = (2 * std::numbers::pi);
    return angle - twoPi * std::round(angle / twoPi);
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

std::pair<double, double> ClosestBar(const Crawler& crawler)
{
    const CollegeBar* closestBar = nullptr;
    double closestDistance = INFINITY;
    for (auto& bar : Map::GetBars())
    {
        double distance = Distance(crawler.xPos, crawler.yPos, bar.pos);
        if (distance < closestDistance)
        {
            closestBar = &bar;
            closestDistance = distance;
        }
    }

    double absDirection = atan2(closestBar->pos.y - crawler.yPos, closestBar->pos.x - crawler.xPos) - crawler.dir;
	return { closestDistance, NormalizeAngle(absDirection) };
}
