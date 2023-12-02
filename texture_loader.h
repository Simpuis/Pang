#pragma once

#include <string>

#include "texture.h"

class texture_loader
{
public:
	static texture* load_texture(const std::string& path, GLenum internal_color_format);
};

