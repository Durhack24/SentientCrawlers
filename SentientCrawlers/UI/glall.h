#pragma once
#include <cassert>

#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif
#include <GL/glew.h>

#include "glerr.h"

#define HALT_ON_ERROR 1

#if HALT_ON_ERROR
#define GlCall(x) GLClearError();\
	x;\
	assert(GLLogCall(#x, __FILE__, __LINE__));
#else
#define GlCall(x) GLClearError();\
	x;\
	GLLogCall(#x, __FILE__, __LINE__);
#endif