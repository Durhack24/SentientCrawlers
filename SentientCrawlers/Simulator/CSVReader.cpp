#include "CSVReader.h"

#include <fstream>
#include <ranges>

std::vector<std::vector<std::string>> CSVReader::Parse(const std::string& path)
{
    std::vector<std::vector<std::string>> result;

    std::ifstream file{ path };
    std::string line;
    while (std::getline(file, line))
        result.push_back(Split(line, ","));

    return result;
}

std::vector<CollegeBar> CSVReader::ParseBars(const std::string& path)
{
    auto values = Parse(path);

    std::vector<CollegeBar> bars;
    bars.reserve(values.size());

    for (auto& value : values)
        bars.emplace_back(value[0], std::stoi(value[1]), std::stoi(value[2]));

    return bars;
}

std::vector<Point> CSVReader::ParsePoints(const std::string& path)
{
    auto values = Parse(path);

    std::vector<Point> points;
    points.reserve(values.size());

    for (auto& value : values)
        points.emplace_back(std::stoi(value[0]), std::stoi(value[1]));

    return points;
}

std::vector<std::string> CSVReader::Split(std::string_view str, std::string_view delim)
{
    std::vector<std::string> split;
    for (auto part : std::ranges::split_view(str, delim))
        split.emplace_back(part.begin(), part.end());
    return split;
}