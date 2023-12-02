#pragma once
#include <glad/glad.h>

class texture
{
public:
	void generate(unsigned int width, unsigned int height, unsigned char* data, GLenum internal_color_format, GLenum source_color_format);

	void bind(int unit = -1);
	void unbind();

private:
	unsigned int id_;
};

