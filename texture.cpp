#include "texture.h"

void texture::generate(unsigned width, unsigned height, unsigned char* data, GLenum internal_color_format,
                       GLenum source_color_format)
{
	glGenTextures(1, &id_);
	glBindTexture(GL_TEXTURE_2D, id_);

	glTexImage2D(GL_TEXTURE_2D, 0, internal_color_format, width, height, 0, source_color_format, GL_UNSIGNED_BYTE,
			data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);	
}

void texture::bind(int unit)
{
	if(unit >= 0) {
		glActiveTexture(GL_TEXTURE0 + unit);
	}
	glBindTexture(GL_TEXTURE_2D, id_);
}

void texture::unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
