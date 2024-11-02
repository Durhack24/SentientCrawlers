#pragma once
#include "Brain.h"

class Crawler
{
public:
	Crawler(double xPos_, double yPos_, double dir_);

    void Step(const std::vector<double>& stimuli, bool inBar);
	void Reset(double xPos_, double yPos_, double dir_);
	int GetBarMinutes();

protected:
	Brain brain;

	double xPos, yPos, dir;
	double intoxication = 0;
	int minutesAtBar = 0;
};