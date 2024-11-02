#pragma once
#include "Brain.h"

class Crawler
{
public:
    double xPos, yPos, dir;

	Crawler(double xPos_, double yPos_, double dir_);

    void Step(const std::vector<double>& stimuli);

	void Reset(double xPos_, double yPos_, double dir_);

protected:
	Brain brain;

	double intoxication = 0;
};