#pragma once

#include <glm/vec3.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <tiny_gltf.h>
#include <flecs.h>

#include "src/serialization/serializable.h"

struct position {
    position() = default;
    explicit position(glm::vec3 pos) : pos(pos) {}
    position(float x, float y, float z) : pos(x, y, z) {}

    glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
};

REFL_TYPE(glm::vec3, bases<>)
    REFL_FIELD(x, serializable())
    REFL_FIELD(y, serializable())
    REFL_FIELD(z, serializable())
REFL_END

REFL_TYPE(position, bases<>)
    REFL_FIELD(pos, serializable())
REFL_END


struct rotation {
    rotation() = default;
    rotation(float x, float y, float z, float w) : rot(w, x, y, z) {}

    glm::quat rot = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
};

REFL_TYPE(glm::quat, bases<>)
        REFL_FIELD(x, serializable())
        REFL_FIELD(y, serializable())
        REFL_FIELD(z, serializable())
        REFL_FIELD(w, serializable())
REFL_END

REFL_TYPE(rotation, bases<>)
        REFL_FIELD(rot, serializable())
REFL_END


struct scale {
    scale() = default;
    scale(float x, float y, float z) : local_scale(x, y, z) {}

    glm::vec3 local_scale = glm::vec3(1.0f, 1.0f, 1.0f);
};

REFL_TYPE(scale, bases<>)
        REFL_FIELD(local_scale, serializable())
REFL_END

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
