#pragma once

#include <string>
#include <fstream>

#include "CollegeBar.h"

class CSVReader
{
public:
    static std::vector<std::vector<std::string>> Parse(const std::string& path);

    static std::vector<CollegeBar> ParseBars(const std::string& path);
protected:
    static std::vector<std::string> Split(std::string_view str, std::string_view delim);
};