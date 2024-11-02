#pragma once
#include <vector>

#include "CollegeBar.h"
#include "RiverPoint.h"

class Map
{
public:
	static const std::vector<CollegeBar>& GetBars();
	static const std::vector<RiverPoint>& GetRiver();
};