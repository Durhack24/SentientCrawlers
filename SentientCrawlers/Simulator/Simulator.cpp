#include "Simulator.h"

Simulator::Simulator(size_t numCrawlers)
{

}

void Simulator::Step(size_t num)
{
	for (size_t stepIdx = 0; stepIdx < num; stepIdx++)
	{
		for (Crawler& crawler : crawlers)
		{
			// Determine stimuli

			// Step the crawler
//			crawler.Step();
		}
	}
}

void Simulator::NextGeneration()
{

}
