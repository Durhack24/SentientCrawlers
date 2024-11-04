#include "colorutils.h"

#include <cmath>
#include <algorithm>

std::tuple<uint8_t, uint8_t, uint8_t> hsv2rgb(float h, float s, float v)
{
    h -= std::floor(h);
    h *= 512;

    float r, g, b;

    int hi = (int) (h / 60.0f) % 6;
    float f  = (h / 60.0f) - hi;
    float p  = v * (1.0f - s);
    float q  = v * (1.0f - s * f);
    float t  = v * (1.0f - s * (1.0f - f));

    switch(hi) {
        case 0: r = v, g = t, b = p; break;
        case 1: r = q, g = v, b = p; break;
        case 2: r = p, g = v, b = t; break;
        case 3: r = p, g = q, b = v; break;
        case 4: r = t, g = p, b = v; break;
        case 5: r = v, g = p, b = q; break;
    }

    return { r * 255, g * 255, b * 255 };
}