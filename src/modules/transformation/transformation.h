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

    inline static std::function<void(deserialization_data& data)> position_deserializer();
    inline static std::function<void(deserialization_data& data)> rotation_deserializer();
    inline static std::function<void(deserialization_data& data)> scale_deserializer();
};
