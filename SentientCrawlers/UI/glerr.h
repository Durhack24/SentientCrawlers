#pragma once
#include "glall.h"
#include <string>

void GLClearError();
bool GLLogCall(const char* func, const char* file, int line);
std::string ErrorCodeToStr(GLenum errorCode);