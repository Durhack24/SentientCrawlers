#include "Crawler.h"

Crawler::Crawler(double xPos_, double yPos_, double dir_)
	: brain(Brain::Random()), xPos(xPos_), yPos(yPos_), dir(dir_)
{

}

void Crawler::Reset(double xPos_, double yPos_, double dir_)
{
	xPos = xPos_;
	yPos = yPos_;
	dir = dir_;
	intoxication = 0;
}