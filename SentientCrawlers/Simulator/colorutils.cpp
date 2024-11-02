#include "colorutils.h"

#include <algorithm>

std::tuple<uint8_t, uint8_t, uint8_t> rgb2hsv(uint8_t src_r, uint8_t src_g, uint8_t src_b)
{
    float r = src_r / 255.0f;
    float g = src_g / 255.0f;
    float b = src_b / 255.0f;

    float h, s, v;

    float max = std::max(r, std::max(g, b));
    float min = std::min(r, std::min(g, b));

    v = max;

    if (max == 0.0f)
    {
        s = 0;
        h = 0;
    }
    else if (max - min == 0.0f)
    {
        s = 0;
        h = 0;
    }
    else
    {
        s = (max - min) / max;

        if (max == r)
            h = 60 * ((g - b) / (max - min)) + 0;
        else if (max == g)
            h = 60 * ((b - r) / (max - min)) + 120;
        else
            h = 60 * ((r - g) / (max - min)) + 240;
    }

    if (h < 0) h += 360.0f;

    return { h / 2, s * 255, v * 255 };
}

std::tuple<uint8_t, uint8_t, uint8_t> hsv2rgb(uint8_t src_h, uint8_t src_s, uint8_t src_v)
{
    float h = src_h * 2.0f;
    float s = src_s / 255.0f;
    float v = src_v / 255.0f;

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