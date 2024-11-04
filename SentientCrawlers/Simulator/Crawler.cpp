#include "Crawler.h"

#include <cmath>
#include <numbers>

#include "Map.h"

Crawler::Crawler(Point pos_, double dir_)
    : pos(pos_), dir(dir_), brain(Brain::Random()) {}

Crawler::Crawler(double xPos, double yPos, double dir_)
	: pos(xPos, yPos), dir(dir_), brain(Brain::Random()) {}

Crawler Crawler::Mutate(const Crawler& c)
{
    Brain newBrain = Brain::Mutate(c.brain);
    return { newBrain, c.pos, c.dir };
}

static inline double NormalizeAngle(double angle)
{
    constexpr double twoPi = (2 * std::numbers::pi);
    return angle - twoPi * std::round(angle / twoPi);
}

void Crawler::Step(const std::vector<double>& stimuli, std::optional<uint32_t> barIdx)
{
    numSteps++;

    // Have a thought
    auto thought = brain.Think(stimuli);

    // Update turn direction
    double turnDel = thought[0] - thought[1];
    dir = NormalizeAngle(dir + turnDel * 6.28);

    // Move forward
    double speed = thought[2] * 30;
    pos.x += cos(dir) * speed;
    pos.y += sin(dir) * speed;

	// Update bar minutes
    if (barIdx.has_value())
    {
        uint32_t newVisitedBars = visitedBars | (1 << barIdx.value());
        if (visitedBars != newVisitedBars)
        {
            intoxication += 1;
            numVisitedBars++;
        }

        visitedBars = newVisitedBars;
        minutesAtBar++;
    }
    else
    {
        intoxication = std::max(intoxication - 0.02, 0.0);
        minutesAtBar = 0;
    }

    // Reset visited bars if visited all
    if (visitedBars == (1 << Map::GetBars().size()) - 1)
        visitedBars = 0;

    totalIntoxication += intoxication;
}

void Crawler::Reset(Point pos_, double dir_)
{
    pos = pos_;
	dir = dir_;
	minutesAtBar = 0;
    visitedBars = 0;
    numVisitedBars = 0;
    intoxication = 0;
    totalIntoxication = 0;
    numSteps = 0;
}

int Crawler::GetBarMinutes() const
{
	return minutesAtBar;
}

double Crawler::GetAverageIntoxication() const
{
    return totalIntoxication / numSteps;
}

Crawler::Crawler(const Brain& brain_, Point pos_, double dir_)
    : pos(pos_), dir(dir_), brain(brain_) {}