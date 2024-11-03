#include "Map.h"

#include "CSVReader.h"
#include "../Resources/ResourceManager.h"

static std::vector<CollegeBar> bars;

void Map::Load()
{
    bars = CSVReader::ParseBars(ResourceManager::GetResourcePath("bars.csv"));
}

const std::vector<CollegeBar>& Map::GetBars()
{
    return bars;
}