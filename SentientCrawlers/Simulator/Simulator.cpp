#include "Simulator.h"

#include <cmath>
#include <numbers>
#include <algorithm>

#include "Map.h"

Simulator::Simulator(size_t numCrawlers, const Point& startPos_, double initialAngle_)
    : startPos(startPos_), initialAngle(initialAngle_)
{
    crawlers.reserve(numCrawlers);
    for (size_t i = 0; i < numCrawlers; ++i)
        crawlers.emplace_back(0, Crawler{ startPos.x, startPos.y, initialAngle });

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
    {
        pool.submit_loop<size_t>(0, crawlers.size(), [this](const size_t i) {
            StepCrawler(crawlers[i].second);
        }).wait();
    }

    UpdateBuf();
}

static double Cost(const Crawler& c)
{
    return c.numVisitedBars * 10 + c.GetAverageIntoxication() * 10;
}

void Simulator::NextGeneration()
{
    // Calculate costs
    for (auto& [cost, crawler] : crawlers)
        cost = Cost(crawler);

    // Sort the crawlers
    std::sort(crawlers.begin(), crawlers.end(), [](const ScoredCrawler& a, const ScoredCrawler& b) {
        return a.first > b.first;
        });

    // Replace bottom half with mutated top half
    size_t parentIdx = 0;
    for (size_t i = crawlers.size() / 2; i < crawlers.size(); i++)
        crawlers[i].second = Crawler::Mutate(crawlers[parentIdx++].second);

    // Reset all crawlers state (except brain)
    for (auto& [cost, crawler] : crawlers)
        crawler.Reset(startPos, initialAngle);

    gen++;
}

std::vector<ScoredCrawler> Simulator::GetCrawlers()
{
    std::lock_guard lock{ bufMutex };
    return crawlersBuf;
}

size_t Simulator::GetGeneration() const
{
    return gen;
}

void Simulator::StepCrawler(Crawler& crawler)
{
    // Determine stimuli
    auto [barDistance, barDir] = ClosestBar(crawler);
    double minutesAtBar = MinutesSpentAtBar(crawler);
    std::vector<double> stimuli{
        barDistance / 250,
        barDir,
        minutesAtBar
        };

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

int Simulator::MinutesSpentAtBar(const Crawler& crawler)
{
    return crawler.minutesAtBar;
}