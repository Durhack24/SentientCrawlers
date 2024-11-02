#pragma once
#include <vector>

#include "CollegeBar.h"
#include "Point.h"

class Map
{
public:
    static void Load();

	static const std::vector<CollegeBar>& GetBars();
	static const std::vector<Point>& GetRiver();
};