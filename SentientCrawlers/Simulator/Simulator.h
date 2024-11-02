#pragma once
#include <vector>
#include <utility>

#include "Crawler.h"
#include "Point.h"

class Simulator
{
public:
	Simulator(size_t numCrawlers, const Point& startPos);

	void Step(size_t num = 1);
	void NextGeneration();

	const std::vector<Crawler>& GetCrawlers() const;

protected:
	std::vector<Crawler> crawlers, crawlersBuf{};

    std::tuple<uint32_t, double, double> ClosestBar(const Crawler& crawler);
    std::pair<double, double> ClosestRiverPoint(const Crawler& crawler);
    int MinutesSpentAtBar(const Crawler& crawler);
    std::pair<double, double> ClosestBridge(const Crawler& crawler);
};