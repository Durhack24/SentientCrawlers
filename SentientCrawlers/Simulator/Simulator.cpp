#include "Simulator.h"

#include <cmath>
#include <numbers>
#include <algorithm>

#include "Map.h"

#define INITIAL_ANGLE 1.57

Simulator::Simulator(size_t numCrawlers, const Point& startPos_)
    : startPos(startPos_)
{
    crawlers.reserve(numCrawlers);
    for (size_t i = 0; i < numCrawlers; ++i)
        crawlers.emplace_back(startPos.x, startPos.y, INITIAL_ANGLE);

    crawlersBuf = crawlers;
}

static inline double NormalizeAngle(double angle)
{
    constexpr double twoPi = (2 * std::numbers::pi);
    return angle - twoPi * std::round(angle / twoPi);
}

static double Distance(const Point& a, const Point& b)
{
	double dx = a.x - b.x;
	double dy = a.y - b.y;
	return std::hypot(dx, dy);
}

static Point ClosestPointOnLine(const Point& a, const Point& p0, const Point& p1)
{
	double w = p1.x - p0.x;
	double h = p1.y - p0.y;
	double dx = a.x - p0.x;
	double dy = a.y - p0.y;

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
        for (Crawler& crawler : crawlers)
            StepCrawler(crawler);

    UpdateBuf();
}

static double Cost(const Crawler& c)
{
    return c.numVisitedBars + c.GetAverageIntoxication() - c.numBeatings;
}

void Simulator::NextGeneration()
{
    std::sort(crawlers.begin(), crawlers.end(), [](const Crawler& a, const Crawler& b) {
        return Cost(a) > Cost(b);
        });

    size_t parentIdx = 0;
    for (size_t i = crawlers.size() / 2; i < crawlers.size(); i++)
        crawlers[i] = Crawler::Mutate(crawlers[parentIdx++]);

    for (auto& crawler : crawlers)
        crawler.Reset(startPos, INITIAL_ANGLE);
}

std::vector<Crawler> Simulator::GetCrawlers() 
{
    std::lock_guard lock{ bufMutex };
    return crawlersBuf;
}

void Simulator::StepCrawler(Crawler& crawler)
{
    // Determine stimuli
    auto [barDistance, barDir] = ClosestBar(crawler);
    double minutesAtBar = MinutesSpentAtBar(crawler);
    auto [riverDistance, riverDir] = ClosestRiverPoint(crawler);
    auto [bridgeDistance, bridgeDir] = ClosestBridge(crawler);
    std::vector<double> stimuli{ barDistance, barDir, minutesAtBar,
        riverDistance, riverDir, bridgeDistance, bridgeDir };

    // Get current bar
    auto currentBarIdx = GetCurrentBar(crawler);

    // Step the crawler
    crawler.Step(stimuli, currentBarIdx);
}

void Simulator::UpdateBuf()
{
    bufMutex.lock();
    crawlersBuf = crawlers;
    bufMutex.unlock();
}

std::optional<uint32_t> Simulator::GetCurrentBar(const Crawler& crawler)
{
    const std::vector<CollegeBar>& bars = Map::GetBars();

    for (size_t i = 0; i < bars.size(); ++i)
        if (Distance(crawler.pos, bars[i].pos) < 16)
            return i;

    return std::nullopt;
}

std::pair<double, double> Simulator::ClosestBar(const Crawler& crawler)
{
    uint32_t closestBarIdx = 0;
    double closestDistance = INFINITY;
    const std::vector<CollegeBar>& bars = Map::GetBars();

    for (size_t i = 0; i < bars.size(); ++i)
    {
        if (crawler.visitedBars & (1 << i))
            continue;

        double distance = Distance(crawler.pos, bars[i].pos);
        if (distance < closestDistance)
        {
            closestBarIdx = i;
            closestDistance = distance;
        }
    }

    const CollegeBar& closestBar = bars[closestBarIdx];
    double absDirection = atan2(closestBar.pos.y - crawler.pos.y, closestBar.pos.x - crawler.pos.x) - crawler.dir;
	return { closestDistance, NormalizeAngle(absDirection) };
}

std::pair<double, double> Simulator::ClosestRiverPoint(const Crawler& crawler)
{
    Point closestPoint;
    double closestDistance = INFINITY;
    const auto& river = Map::GetRiver();
    for (size_t i = 0; i < river.size() - 1; ++i)
    {
        Point point = ClosestPointOnLine(crawler.pos, river[i], river[i + 1]);
        double distance = Distance(crawler.pos, point);
        if (distance < closestDistance)
        {
            closestPoint = point;
            closestDistance = distance;
        }
    }
    double absDirection = atan2(closestPoint.y - crawler.pos.y, closestPoint.x - crawler.pos.x) - crawler.dir;
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
    for (const auto& bridge : Map::GetBridges())
    {
        double distance = Distance(crawler.pos, bridge);
        if (distance < closestDistance)
        {
            closestBridge = &bridge;
            closestDistance = distance;
        }
    }

    double absDirection = atan2(closestBridge->y - crawler.pos.y, closestBridge->x - crawler.pos.x) - crawler.dir;
    return { closestDistance, NormalizeAngle(absDirection) };
}