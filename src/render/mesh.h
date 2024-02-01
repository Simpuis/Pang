#pragma once

#include <vector>
#include <map>
#include <memory>
#include <tiny_gltf.h>
#include <flecs.h>
#include <glad/glad.h>

class material;

/**
 * @brief A primitive holds the VAO, a pointer to the material and some general info about the mesh for the renderer
 *
 */
struct primitive {
    unsigned int VBO = 0;
    unsigned int VAO = 0;
    unsigned int EBO = 0;

    std::map<std::string, int> attributes;
    unsigned int indices;

    size_t count;
    size_t byte_offset;
    unsigned int indices_component_type;
    unsigned int mode;

    unsigned int material_index;
};

/**
 * @brief A mesh is a collection of primitives, each primitive has a material and a VAO
 *
 */
class mesh {
public:

public:
    std::vector<primitive> primitives;

    tinygltf::Mesh serialize();
    static std::shared_ptr<mesh> deserialize(const tinygltf::Model& model, const tinygltf::Mesh& mesh_to_load);

private:
    void setup_gltf_mesh(const tinygltf::Model& model, const tinygltf::Mesh& mesh_to_load);
};
