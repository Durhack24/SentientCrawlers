#pragma once
#include <string>

#include "Point.h"

struct CollegeBar {
    CollegeBar(const std::string& name_, int xPos, int yPos);

    std::string name;
    Point pos;
};