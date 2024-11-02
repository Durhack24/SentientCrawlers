#pragma once
#include <cstdint>
#include <string>

#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>

bool InitializeGlfw();
std::pair<GLFWwindow*, bool> CreateWindow(uint32_t w, uint32_t h, const std::string& title);
bool InitializeImGui(GLFWwindow* window);