#include "Simulator.h"

#include <cmath>
#include <numbers>

#include "Map.h"

Simulator::Simulator(size_t numCrawlers, const Point& startPos)
{
    crawlers.reserve(numCrawlers);
    for (size_t i = 0; i < numCrawlers; ++i)
        crawlers.emplace_back(startPos.x, startPos.y, 0.0);

    crawlersBuf = crawlers;
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

static Point ClosestPointOnLine(double x, double y, const Point& p0, const Point& p1)
{
	double w = p1.x - p0.x;
	double h = p1.y - p0.y;
	double dx = x - p0.x;
	double dy = y - p0.y;

	double t = (dx * w + dy * h) / (w * w + h * h);

	if (t <= 0)
		return p0;
	if (t >= 1)
		return p1;

	double nearX = p0.x + w * t;
	double nearY = p0.y + h * t;
	return Point{ nearX, nearY };
}

void Simulator::Step(size_t num)
{
	for (size_t stepIdx = 0; stepIdx < num; stepIdx++)
	{
		for (Crawler& crawler : crawlers)
		{
			// Determine stimuli
            auto [barDistance, barDir] = ClosestBar(crawler);
            double minutesAtBar = MinutesSpentAtBar(crawler);
            auto [riverDistance, riverDir] = ClosestRiverPoint(crawler);
            auto [bridgeDistance, bridgeDir] = ClosestBridge(crawler);
            std::vector<double> stimuli{ barDistance, barDir, minutesAtBar,
                riverDistance, riverDir, bridgeDistance, bridgeDir };

			// Step the crawler
			crawler.Step(stimuli, barDistance < 16);
		}
	}

    crawlersBuf = crawlers;
}

void Simulator::NextGeneration()
{

}

const std::vector<Crawler>& Simulator::GetCrawlers() const
{
    return crawlersBuf;
}

std::pair<double, double> Simulator::ClosestBar(const Crawler& crawler)
{
    const Point* closestBar = nullptr;
    double closestDistance = INFINITY;
    for (auto& bar : Map::GetBars())
    {
        double distance = Distance(crawler.xPos, crawler.yPos, bar.pos);
        if (distance < closestDistance)
        {
            closestBar = &bar.pos;
            closestDistance = distance;
        }
    }

    double absDirection = atan2(closestBar->y - crawler.yPos, closestBar->x - crawler.xPos) - crawler.dir;
	return { closestDistance, NormalizeAngle(absDirection) };
}

std::pair<double, double> Simulator::ClosestRiverPoint(const Crawler& crawler)
{
    Point closestPoint;
    double closestDistance = INFINITY;
    const auto& river = Map::GetRiver();
    for (size_t i = 0; i < river.size() - 1; ++i)
    {
        Point point = ClosestPointOnLine(crawler.xPos, crawler.yPos, river[i], river[i + 1]);
        double distance = Distance(crawler.xPos, crawler.yPos, point);
        if (distance < closestDistance)
        {
            closestPoint = point;
            closestDistance = distance;
        }
    }
    double absDirection = atan2(closestPoint.y - crawler.yPos, closestPoint.x - crawler.xPos) - crawler.dir;
    return { closestDistance, NormalizeAngle(absDirection) };
}

int Simulator::MinutesSpentAtBar(const Crawler& crawler)
{
    return crawler.minutesAtBar;
}

std::pair<double, double> Simulator::ClosestBridge(const Crawler& crawler)
{
    const Point* closestBridge = nullptr;
    double closestDistance = INFINITY;
    for (auto& bridge : Map::GetBridges())
    {
        double distance = Distance(crawler.xPos, crawler.yPos, bridge);
        if (distance < closestDistance)
        {
            closestBridge = &bridge;
            closestDistance = distance;
        }
    }

    double absDirection = atan2(closestBridge->y - crawler.yPos, closestBridge->x - crawler.xPos) - crawler.dir;
    return { closestDistance, NormalizeAngle(absDirection) };
}
