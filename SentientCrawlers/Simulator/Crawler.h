#pragma once
#include "Brain.h"
#include "Point.h"

class Crawler
{
public:
    Point pos;
    double dir;
    int minutesAtBar = 0;
    uint32_t visitedBars = 0;
    int numVisitedBars = 0;

    Crawler(Point pos_, double dir_);
	Crawler(double xPos, double yPos, double dir_);

    static Crawler Mutate(const Crawler& c);

    void Step(const std::vector<double>& stimuli, bool inBar, uint32_t barIdx);
	void Reset(Point pos_, double dir_);
	int GetBarMinutes();

protected:
	Brain brain;
	double intoxication = 0;

    Crawler(const Brain& brain_, Point pos_, double dir_);
};