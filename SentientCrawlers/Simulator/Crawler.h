#pragma once
#include "Brain.h"

class Crawler
{
public:
    double xPos, yPos, dir;

	Crawler(double xPos_, double yPos_, double dir_);

    void Step(const std::vector<double>& stimuli, bool inBar);
	void Reset(double xPos_, double yPos_, double dir_);
	int GetBarMinutes();

protected:
	Brain brain;

	double intoxication = 0;
	int minutesAtBar = 0;
};