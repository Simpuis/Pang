#pragma once

#include <string>
#include <memory>
#include <tiny_gltf.h>
#include <glad/glad.h>

class texture
{
public:
    void bind(int unit = -1);
    void unbind();

    static std::shared_ptr<texture> deserialize(const tinygltf::Model& model, const tinygltf::Texture& gltf_texture);

private:
    unsigned int id_;
};
