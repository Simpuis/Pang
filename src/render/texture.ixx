module;

#include <string>
#include <memory>
#include <tiny_gltf.h>
#include <glad/glad.h>

export module texture;

export class texture
{
public:
	void bind(int unit = -1);
	void unbind();

    static std::shared_ptr<texture> deserialize(const tinygltf::Model& model, const tinygltf::Texture& gltf_texture);

private:
	unsigned int id_;
};

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

std::shared_ptr<texture> texture::deserialize(const tinygltf::Model &model, const tinygltf::Texture &gltf_texture) {
    auto tex = std::make_shared<texture>();

    const auto& image = model.images[gltf_texture.source];

    glGenTextures(1, &tex->id_);
    glBindTexture(GL_TEXTURE_2D, tex->id_);

    {
        const auto &sampler = model.samplers[gltf_texture.sampler];
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, sampler.wrapS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, sampler.wrapT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, sampler.minFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, sampler.magFilter);
    }

    GLenum format;
    if(image.component == 1) {
        format = GL_RED;
    }
    else if(image.component == 2) {
        format = GL_RG;
    }
    else if(image.component == 3) {
        format = GL_RGB;
    }
    else if(image.component == 4) {
        format = GL_RGBA;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width, image.height, 0, format, image.pixel_type,
                 &image.image.at(0));
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    return tex;
}
