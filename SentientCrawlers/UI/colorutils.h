#pragma once
#include <cstdint>
#include <utility>

std::tuple<uint8_t, uint8_t, uint8_t> hsv2rgb(float h, float s, float v);