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

    void save(tinygltf::Node& node) {
        node.translation.push_back(pos.x);
        node.translation.push_back(pos.y);
        node.translation.push_back(pos.z);
    }

    void load(const tinygltf::Node& node) {
        pos = glm::vec3(node.translation[0]);
        pos = glm::vec3(node.translation[1]);
        pos = glm::vec3(node.translation[2]);
    }
};


struct rotation {
    glm::quat rot = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

    void save(tinygltf::Node& node) {
        node.rotation.push_back(rot.x);
        node.rotation.push_back(rot.y);
        node.rotation.push_back(rot.z);
        node.rotation.push_back(rot.w);
    }

    void load(const tinygltf::Node& node) {
        rot = glm::quat(node.rotation[3], node.rotation[0], node.rotation[1], node.rotation[2]);
    }
};


struct scale {
    glm::vec3 local_scale = glm::vec3(1.0f, 1.0f, 1.0f);

    void save(tinygltf::Node& node) {
        node.scale.push_back(local_scale.x);
        node.scale.push_back(local_scale.y);
        node.scale.push_back(local_scale.z);
    }

    void load(const tinygltf::Node& node) {
        local_scale = glm::vec3(node.scale[0], node.scale[1], node.scale[2]);
    }
};


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
    world.component<glm::vec3>()
            .member<float>("x")
            .member<float>("y")
            .member<float>("z");
    world.component<glm::quat>()
            .member<float>("x")
            .member<float>("y")
            .member<float>("z")
            .member<float>("w");
    world.component<position>("position")
            .member<glm::vec3>("pos");
    world.component<rotation>("rotation")
            .member<glm::quat>("rot");
    world.component<scale>("scale")
            .member<glm::vec3>("local_scale");
}
