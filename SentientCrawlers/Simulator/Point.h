#pragma once

struct Point
{
    Point() = default;
    Point(int x_, int y_);
    Point(double x_, double y_);

	double x, y;
};