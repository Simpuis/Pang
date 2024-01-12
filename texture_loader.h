#pragma once

#include <string>
#include <memory>

#include "texture.h"

class texture_loader
{
public:
	static std::shared_ptr<texture> load_texture(const std::string& path, GLenum internal_color_format);
};

