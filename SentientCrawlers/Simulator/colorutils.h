#pragma once

#include <cstdint>
#include <utility>

std::tuple<uint8_t, uint8_t, uint8_t> rgb2hsv(uint8_t src_r, uint8_t src_g, uint8_t src_b);

std::tuple<uint8_t, uint8_t, uint8_t> hsv2rgb(uint8_t src_h, uint8_t src_s, uint8_t src_v);