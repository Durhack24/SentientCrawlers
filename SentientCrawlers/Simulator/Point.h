#pragma once
#include <cstdint>

struct Point
{
    Point() = default;
    Point(int x_, int y_);
    Point(uint32_t x_, uint32_t y_);
    Point(double x_, double y_);

	double x, y;
};