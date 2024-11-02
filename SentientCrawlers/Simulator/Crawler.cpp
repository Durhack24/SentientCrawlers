#include "Crawler.h"

Crawler::Crawler(double xPos_, double yPos_, double dir_)
	: brain(Brain::Random()), xPos(xPos_), yPos(yPos_), dir(dir_) {}

void Crawler::Step(const std::vector<double>& stimuli, bool inBar)
{
	// Have a thought
	auto thought = brain.Think(stimuli);

	// Update turn direction
	double turnDel = thought[0] - thought[1];
	dir += turnDel * 0.25;

	// Move forward
	double speed = thought[2] * 30;
	xPos += cos(dir) * speed;
	yPos += sin(dir) * speed;

	// Update bar minutes
	if (inBar)
		minutesAtBar++;
	else
		minutesAtBar = 0;
}

void Crawler::Reset(double xPos_, double yPos_, double dir_)
{
	xPos = xPos_;
	yPos = yPos_;
	dir = dir_;
	intoxication = 0;
	minutesAtBar = 0;
}

int Crawler::GetBarMinutes()
{
	return minutesAtBar;
}