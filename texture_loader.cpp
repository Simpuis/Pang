#include <stb_image.h>

#include "texture.h"
#include "texture_loader.h"

#include <stdexcept>

texture* texture_loader::load_texture(const std::string& path, GLenum internal_color_format)
{
	int width, height, number_channels;

	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &number_channels, 0);

	GLenum color_format = GL_RGB;
	switch (number_channels)
	{
	case 1:
		color_format = GL_RED;
		break;

	case 3:
		color_format = GL_RGB;
		break;

	case 4:
		color_format = GL_RGBA;
		break;
	}

	if(data) {
		texture* loaded_texture = new texture();
		loaded_texture->generate(width, height, data, internal_color_format, color_format);

		stbi_image_free(data);
		return loaded_texture;
	}
	else
	{
		stbi_image_free(data);
		throw std::runtime_error("Failed to load texture");
	}
}
