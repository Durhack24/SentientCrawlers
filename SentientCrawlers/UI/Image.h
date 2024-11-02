#pragma once
#include <cstdint>
#include <string>

#include "glall.h"

class Image
{
public:
	Image(const std::string& path);

	uint32_t Width() const;
	uint32_t Height() const;
	GLuint Id() const;

protected:
	uint32_t w, h;
	GLuint id;
};