#pragma once
#include "Brain.h"

class Crawler
{
public:
	Crawler(double xPos_, double yPos_, double dir_);

	void Reset(double xPos_, double yPos_, double dir_);

protected:
	Brain brain;

	double xPos, yPos, dir;
	double intoxication = 0;
};