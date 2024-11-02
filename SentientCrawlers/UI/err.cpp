#include "err.h"

#include <iostream>
#include <format>

void GlfwErrorCallback(int error, const char* description)
{
    std::cerr << std::format("GLFW Error {}: {}\n", error, description);
}