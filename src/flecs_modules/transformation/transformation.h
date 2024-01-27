#pragma once

#include <glm/vec3.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <tiny_gltf.h>
#include <flecs.h>

struct position {
    position() = default;
    explicit position(glm::vec3 pos) : pos(pos) {}
    position(float x, float y, float z) : pos(x, y, z) {}

    glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
};

template<typename Archive>
void serialize(Archive& archive, tinygltf::Node& node, position& position_comp) {
    archive(node.translation[0], position_comp.pos.x);
    archive(node.translation[1], position_comp.pos.y);
    archive(node.translation[2], position_comp.pos.z);
}

struct rotation {
    glm::quat rot = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
};

template<typename Archive>
void serialize(Archive& archive, tinygltf::Node& node, rotation& rotation_comp) {
    archive(node.rotation[0], rotation_comp.rot.x);
    archive(node.rotation[1], rotation_comp.rot.y);
    archive(node.rotation[2], rotation_comp.rot.z);
    archive(node.rotation[3], rotation_comp.rot.w);
}

struct scale {
    glm::vec3 local_scale = glm::vec3(1.0f, 1.0f, 1.0f);
};

template<typename Archive>
void serialize(Archive& archive, tinygltf::Node& node, scale& scale_comp) {
    archive(node.scale[0], scale_comp.local_scale.x);
    archive(node.scale[1], scale_comp.local_scale.y);
    archive(node.scale[2], scale_comp.local_scale.z);
}

struct transformation {
    explicit transformation(flecs::world& world);

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

inline transformation::transformation(flecs::world &world) {
    world.component<position>("position");
    world.component<rotation>("rotation");
    world.component<scale>("scale");
}
