#include "Map.h"

#include "CSVReader.h"
#include "../Resources/ResourceManager.h"

static std::vector<CollegeBar> bars;
static std::vector<RiverPoint> river;

void Map::Load()
{
    bars = CSVReader::ParseBars(ResourceManager::GetResourcePath("bars.csv"));
    river = CSVReader::ParseRiverPoints(ResourceManager::GetResourcePath("river.csv"));
}

const std::vector<CollegeBar>& Map::GetBars()
{
    return bars;
}

const std::vector<RiverPoint>& Map::GetRiver()
{
    return river;
}