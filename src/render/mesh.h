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

    size_t count;
    size_t byte_offset;
    unsigned int indices_component_type;
    unsigned int mode;

    std::shared_ptr<material> mat;
};

/**
 * @brief A mesh is a collection of primitives, each primitive has a material and a VAO
 *
 */
class mesh {
public:

public:
    std::vector<primitive> primitives;

    static void deserialize(const tinygltf::Model& model, const tinygltf::Node& node,
                            std::map<unsigned int, std::shared_ptr<material>>& material_lookup, flecs::entity& entity);

private:
    void setup_gltf_mesh(const tinygltf::Model& model, const tinygltf::Mesh& mesh_to_load, std::map<unsigned int, std::shared_ptr<material>>& material_lookup);
};
