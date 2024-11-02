#include "Map.h"

#include "CSVReader.h"
#include "../Resources/ResourceManager.h"

static std::vector<CollegeBar> bars = CSVReader::ParseBars(ResourceManager::GetResourcePath("bars.csv"));
static std::vector<RiverPoint> river = CSVReader::ParseRiverPoints(ResourceManager::GetResourcePath("river.csv"));


const std::vector<CollegeBar>& Map::GetBars()
{
    return bars;
}

const std::vector<RiverPoint>& Map::GetRiver()
{
    return river;
}
