#include <gsl/util>
#include "texture.h"

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

tinygltf::Texture texture::serialize() {
    tinygltf::Texture serialized_texture;

    serialized_texture.source = gsl::narrow_cast<int>(source_index);
    serialized_texture.sampler = gsl::narrow_cast<int>(sampler_index);
    serialized_texture.name = name;

    return serialized_texture;
}

std::shared_ptr<texture> texture::deserialize(const tinygltf::Model &model, const tinygltf::Texture &gltf_texture) {
    auto tex = std::make_shared<texture>();
    tex->source_index = gltf_texture.source;
    tex->sampler_index = gltf_texture.sampler;
    tex->name = gltf_texture.name;
    const auto& image = model.images[gltf_texture.source];

    glGenTextures(1, &tex->id_);
    glBindTexture(GL_TEXTURE_2D, tex->id_);

    configure_sampling(model.samplers[gltf_texture.sampler]);

    GLenum format = get_texture_format(image.component);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width, image.height, 0, format, image.pixel_type,
                 &image.image.at(0));
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    return tex;
}

void texture::configure_sampling(const tinygltf::Sampler &sampler) {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, sampler.wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, sampler.wrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, sampler.minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, sampler.magFilter);
}

GLenum texture::get_texture_format(int component) {
    GLenum format;
    if(component == 1) {
        format = GL_RED;
    }
    else if(component == 2) {
        format = GL_RG;
    }
    else if(component == 3) {
        format = GL_RGB;
    }
    else if(component == 4) {
        format = GL_RGBA;
    }
    return format;
}
