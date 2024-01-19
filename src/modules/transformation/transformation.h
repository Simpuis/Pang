#pragma once

#include <glm/vec3.hpp>
#include <glm/ext/quaternion_float.hpp>

#include "src/serialization/scene_deserializer.h"

namespace flecs {
    struct world;
}

struct position {
    glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
};

struct rotation {
    glm::quat rot = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
};

struct scale {
    glm::vec3 local_scale = glm::vec3(1.0f, 1.0f, 1.0f);
};

struct transformation {
    explicit transformation(flecs::world& world);

    inline static std::function<void(const tinygltf::Model&, const tinygltf::Node&, flecs::entity&)> position_deserializer();
    inline static std::function<void(const tinygltf::Model&, const tinygltf::Node&, flecs::entity&)> rotation_deserializer();
    inline static std::function<void(const tinygltf::Model&, const tinygltf::Node&, flecs::entity&)> scale_deserializer();

    template<typename Deserializer_T>
    static void register_deserializers(Deserializer_T& deserializer) {
        deserializer([](const tinygltf::Node& node){ return !node.translation.empty(); }, position_deserializer());
        deserializer([](const tinygltf::Node& node){ return !node.rotation.empty(); }, rotation_deserializer());
        deserializer([](const tinygltf::Node& node){ return !node.scale.empty(); }, scale_deserializer());
    }
};

std::function<void(const tinygltf::Model&, const tinygltf::Node&, flecs::entity&)> transformation::position_deserializer() {
    return [] (const tinygltf::Model& model, const tinygltf::Node& node, flecs::entity& entity) {
        if(node.translation.empty()) return;

        position position_comp;
        position_comp.pos = glm::vec3(node.translation[0], node.translation[1], node.translation[2]);

        entity.set<position>(position_comp);
    };
}

std::function<void(const tinygltf::Model&, const tinygltf::Node&, flecs::entity&)> transformation::rotation_deserializer() {
    return [] (const tinygltf::Model& model, const tinygltf::Node& node, flecs::entity& entity) {
        if(node.rotation.empty()) return;

        rotation rotation_comp;
        rotation_comp.rot = glm::quat(node.rotation[3], node.rotation[0], node.rotation[1], node.rotation[2]);

        entity.set<rotation>(rotation_comp);
    };
}

std::function<void(const tinygltf::Model&, const tinygltf::Node&, flecs::entity&)> transformation::scale_deserializer() {
    return [] (const tinygltf::Model& model, const tinygltf::Node& node, flecs::entity& entity) {
        if(node.scale.empty()) return;

        scale scale_comp;
        scale_comp.local_scale = glm::vec3(node.scale[0], node.scale[1], node.scale[2]);

        entity.set<scale>(scale_comp);
    };
}
