#pragma once

#include <glm/vec3.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

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

    inline static std::function<position(const tinygltf::Node&)> position_deserializer();
    inline static std::function<rotation(const tinygltf::Node&)> rotation_deserializer();
    inline static std::function<scale(const tinygltf::Node&)> scale_deserializer();

    template<typename Deserializer_T>
    static void register_deserializers(Deserializer_T& deserializer) {
        deserializer([](const tinygltf::Node& node){ return !node.translation.empty(); }, position_deserializer());
        deserializer([](const tinygltf::Node& node){ return !node.rotation.empty(); }, rotation_deserializer());
        deserializer([](const tinygltf::Node& node){ return !node.scale.empty(); }, scale_deserializer());
    }

    inline static glm::mat4x4 model(const position& pos, const rotation& rot, const scale& scale) {
        glm::mat4x4 transform = glm::mat4x4(1.0f);
        transform = glm::translate(transform, pos.pos);
        transform *= glm::mat4_cast(rot.rot);
        transform = glm::scale(transform, scale.local_scale);

        return transform;
    }

    inline constexpr static glm::vec3 global_forward() {
        constexpr glm::vec3 global_forward = glm::vec3(0.0f, 0.0f, -1.0f);
        return global_forward;
    }
    inline static glm::vec3 forward(const position& pos, const rotation& rot, const scale& scale) {
        return model(pos, rot, scale) * glm::vec4(global_forward(), 0.0f);
    }

    inline constexpr static glm::vec3 global_up() {
        constexpr glm::vec3 global_up = glm::vec3(0.0f, 1.0f, 0.0f);
        return global_up;
    }
    inline static glm::vec3 up(const position& pos, const rotation& rot, const scale& scale) {
        return model(pos, rot, scale) * glm::vec4(global_up(), 0.0f);
    }

    inline constexpr static glm::vec3 global_right() {
        constexpr glm::vec3 global_right = glm::vec3(-1.0f, 0.0f, 0.0f);
        return global_right;
    }
    inline static glm::vec3 right(const position& pos, const rotation& rot, const scale& scale) {
        return model(pos, rot, scale) * glm::vec4(global_right(), 0.0f);
    }
};

std::function<position(const tinygltf::Node&)> transformation::position_deserializer() {
    return [] (const tinygltf::Node& node) {
        position position_comp;
        position_comp.pos = glm::vec3(node.translation[0], node.translation[1], node.translation[2]);

        return position_comp;
    };
}

std::function<rotation(const tinygltf::Node&)> transformation::rotation_deserializer() {
    return [] (const tinygltf::Node& node) {
        rotation rotation_comp;
        rotation_comp.rot = glm::quat(node.rotation[3], node.rotation[0], node.rotation[1], node.rotation[2]);

        return rotation_comp;
    };
}
std::function<scale(const tinygltf::Node&)> transformation::scale_deserializer() {
    return [] (const tinygltf::Node& node) {
        scale scale_comp;
        scale_comp.local_scale = glm::vec3(node.scale[0], node.scale[1], node.scale[2]);

        return scale_comp;
    };
}
