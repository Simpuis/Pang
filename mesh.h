#pragma once

#include <vector>
#include <entt/entt.hpp>

class scene_serializer;
class material;

struct deserialization_data;

namespace tinygltf {
    class Node;
    class Model;
    class Mesh;
}

struct primitive {
    unsigned int VBO = 0;
    unsigned int VAO = 0;
    unsigned int EBO = 0;

    size_t count;
    size_t byte_offset;

    std::shared_ptr<material> mat;
};

class mesh {
public:

public:
    std::vector<primitive> primitives;

    void serialize(scene_serializer& serializer, tinygltf::Model& model, tinygltf::Node* node) const;
    static void deserialize(deserialization_data& data);

private:
    void setup_gltf_mesh(const tinygltf::Model& model, const tinygltf::Mesh& mesh_to_load, std::map<unsigned int, std::shared_ptr<material>>& material_lookup);
};
