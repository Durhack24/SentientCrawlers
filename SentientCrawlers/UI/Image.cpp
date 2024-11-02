#include "Image.h"

#include "stb_image.h"

Image::Image(const std::string& path)
{
    unsigned char *data = stbi_load(path.c_str(), (int*)&w, (int*)&h, nullptr, 4);

    // Generate texture
    GlCall(glGenTextures(1, &id));

    // Bind texture
    GlCall(glBindTexture(GL_TEXTURE_2D, id));

    // Update texture parameters
    GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

    // Copy over data
    GlCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));

    stbi_image_free(data);
}

uint32_t Image::Width() const
{
	return w;
}

uint32_t Image::Height() const
{
	return h;
}

GLuint Image::Id() const
{
    return id;
}