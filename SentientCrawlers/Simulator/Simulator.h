#pragma once
#include <vector>

#include "Crawler.h"

class Simulator
{
public:
	Simulator(size_t numCrawlers);

	void Step(size_t num = 1);
	void NextGeneration();

protected:
	std::vector<Crawler> crawlers;

	double ClosestBar(const Crawler& crawler);
	double ClosestBarDir(const Crawler& crawler);
};