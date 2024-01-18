#pragma once

#include "src/modules/transformation/transformation.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

class transformations {
public:
    transformations() = delete;

public:
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
