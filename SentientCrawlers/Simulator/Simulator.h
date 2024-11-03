#pragma once
#include <vector>
#include <utility>
#include <mutex>

#include "Crawler.h"
#include "Point.h"

using ScoredCrawler = std::pair<double, Crawler>;

class Simulator
{
public:
	Simulator(size_t numCrawlers, const Point& startPos_);

	void Step(size_t num = 1);
	void NextGeneration();

	std::vector<ScoredCrawler> GetCrawlers();
	static void StepCrawler(Crawler& crawler);

	void UpdateBuf();

protected:
	std::mutex bufMutex;
	std::vector<ScoredCrawler> crawlers, crawlersBuf{};
	Point startPos;

    static std::optional<uint32_t> GetCurrentBar(const Crawler& crawler);
    static std::pair<double, double> ClosestBar(const Crawler& crawler);
    static std::pair<double, double> ClosestRiverPoint(const Crawler& crawler);
    static int MinutesSpentAtBar(const Crawler& crawler);
    static std::pair<double, double> ClosestBridge(const Crawler& crawler);
};