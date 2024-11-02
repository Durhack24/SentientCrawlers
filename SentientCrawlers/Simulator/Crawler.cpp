#include "Crawler.h"

#include "Map.h"

Crawler::Crawler(Point pos_, double dir_)
    : pos(pos_), dir(dir_), brain(Brain::Random()) {}

Crawler::Crawler(double xPos, double yPos, double dir_)
	: pos(xPos, yPos), dir(dir_), brain(Brain::Random()) {}

void Crawler::Step(const std::vector<double>& stimuli, bool inBar, uint32_t barIdx)
{
    // Have a thought
    auto thought = brain.Think(stimuli);

    // Update turn direction
    double turnDel = thought[0] - thought[1];
    dir += turnDel * 0.25;

    // Move forward
    double speed = thought[2] * 30;
    pos.x += cos(dir) * speed;
    pos.y += sin(dir) * speed;

    uint32_t newVisitedBars = visitedBars | (1 << barIdx);
    if (visitedBars != newVisitedBars)
    {
        numVisitedBars++;
        visitedBars = newVisitedBars;
    }

    if (visitedBars == (1 << Map::GetBars().size()) - 1)
        visitedBars = 0;

	// Update bar minutes
	if (inBar)
		minutesAtBar++;
	else
		minutesAtBar = 0;
}

void Crawler::Reset(double xPos_, double yPos_, double dir_)
{
	pos.x = xPos_;
	pos.y = yPos_;
	dir = dir_;
	intoxication = 0;
	minutesAtBar = 0;
}

int Crawler::GetBarMinutes()
{
	return minutesAtBar;
}