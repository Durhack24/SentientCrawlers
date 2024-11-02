#include "glerr.h"
#include <iostream>
#include <format>

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* func, const char* file, int line)
{
    while (GLenum errorCode = glGetError())
    {
        std::cerr << std::format("[OpenGL Error] ({}): {} AT {}:{}\n", ErrorCodeToStr(errorCode), func, file, line);
        return false;
    }
    return true;
}

std::string ErrorCodeToStr(GLenum errorCode)
{
    switch (errorCode)
    {
    case GL_INVALID_ENUM:                   return "INVALID_ENUM";
    case GL_INVALID_VALUE:                  return "INVALID_VALUE";
    case GL_INVALID_OPERATION:              return "INVALID_OPERATION";
    case GL_STACK_OVERFLOW:                 return "STACK_OVERFLOW";
    case GL_STACK_UNDERFLOW:                return "STACK_UNDERFLOW";
    case GL_OUT_OF_MEMORY:                  return "OUT_OF_MEMORY";
    case GL_INVALID_FRAMEBUFFER_OPERATION:  return "INVALID_FRAMEBUFFER_OPERATION";
    default:                                return "UNKNOWN ERROR";
    }
}