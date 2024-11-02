#pragma once
#include <vector>
#include <utility>
#include <mutex>

#include "Crawler.h"
#include "Point.h"

class Simulator
{
public:
	Simulator(size_t numCrawlers, const Point& startPos_);

	void Step(size_t num = 1);
	void NextGeneration();

	std::vector<Crawler> GetCrawlers();

protected:
	std::mutex bufMutex;
	std::vector<Crawler> crawlers, crawlersBuf{};
	Point startPos;

    std::tuple<uint32_t, double, double> ClosestBar(const Crawler& crawler);
    std::pair<double, double> ClosestRiverPoint(const Crawler& crawler);
    int MinutesSpentAtBar(const Crawler& crawler);
    std::pair<double, double> ClosestBridge(const Crawler& crawler);

	void UpdateBuf();
};